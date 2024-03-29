#include "uplift_desk.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"
#include <vector>

namespace esphome {
namespace uplift_desk {

static const char *TAG = "uplift_desk";

const uint8_t STATE_IDLE = 0;
const uint8_t STATE_MOVING_UP = 1;
const uint8_t STATE_MOVING_DOWN = 2;

void log_buffer(uint8_t *buffer, uint8_t eot_index) {
  for (uint8_t i = 0; i <= eot_index; i++) {
    ESP_LOGV(TAG, "  i=%u: 0b" BYTE_TO_BINARY_PATTERN " (0x%02X, %i)", i, BYTE_TO_BINARY(buffer[i]),
             buffer[i], buffer[i]);
  }
}

UpliftDeskComponent::UpliftDeskComponent() : uart::UARTDevice() {};

void UpliftDeskComponent::setup() {
   this->defer([this]() { this->send_cmd_sync(); });
}

void UpliftDeskComponent::loop() {
  const uint32_t now = millis();
  if (now - this->last_transmission_ >= 500) {
    // last transmission too long ago. Reset RX index.
    this->reset_buffer_();
  }

  if (!this->available())
    return;

  this->last_transmission_ = now;
  while (this->available()) {
    this->read_byte(&this->buffer_[this->buffer_index_]);
    if (!this->check_byte_()) {
      this->reset_buffer_();
      this->status_set_warning();
    }

    if (this->buffer_index_ == this->eot_index_) {
      this->parse_data_();
      this->status_clear_warning();
      this->reset_buffer_();
      return;
    } else {
      this->buffer_index_ = (this->buffer_index_ + 1) % UPLIFT_DESK_BUFFER_LENGTH;
    }
  }
}

bool UpliftDeskComponent::check_byte_() {
  const uint8_t index = this->buffer_index_;
  const uint8_t byte = this->buffer_[index];

  if (index == 0 || index == 1 ) {
    return byte == 0xF2;
  }

  if (index == 3) {
    this->eot_index_ = index + byte + 2;  // CRC and EOT not included
    return true;
  }

  if (index == this->eot_index_ - 1) {
    const uint8_t crc_index = this->eot_index_ - 1;
    uint8_t crc = 0;
    for (uint8_t i = 2; i < crc_index; i++) {
      crc += this->buffer_[i];
    }
    if (crc != this->buffer_[crc_index]) {
      ESP_LOGW(TAG, "Invalid checksum: got 0x%02X, expected 0x%02X", this->buffer_[crc_index], crc);
      log_buffer(this->buffer_, this->eot_index_);
      return false;
    }
    return true;
  }

  if (index == this->eot_index_) {
    return byte == 0x7E;
  }

  return true;
}

void UpliftDeskComponent::parse_data_() {
  switch (this->buffer_[2]) {
    case 0x01: { // Height report
      float inches = ((this->buffer_[4] << 8) | this->buffer_[5]) / 10.0;
      ESP_LOGV(TAG, "Height report: [ %d, %d ], %.1f\"", this->buffer_[4], this->buffer_[5], inches);

      if (this->state_sensor_ != nullptr) {
        uint8_t state;
        if (inches > this->height_sensor_->state) {
          state = STATE_MOVING_UP;
        } else if (inches < this->height_sensor_->state) {
          state = STATE_MOVING_DOWN;
        } else {
          state = STATE_IDLE;
        }
        if (state != this->state_sensor_->state)
          this->state_sensor_->publish_state(state);
      }
      if (this->height_sensor_ != nullptr)
        this->height_sensor_->publish_state(inches);
      break;
    }
    case 0x20: // Height limit response
      switch (this->buffer_[4]) {
        case 0x00: // Cleared
          ESP_LOGV(TAG, "Height limits cleared.");
          break;
        case 0x01: // Max set
          ESP_LOGV(TAG, "Height max limit set.");
          break;
        case 0x10: // Min set
          ESP_LOGV(TAG, "Height min limit set.");
          break;
      }
      break;
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28: { // Preset height value response
      uint8_t preset = (this->buffer_[2] - 0x25) + 1;
      ESP_LOGV(TAG, "Preset %d height: [ %d, %d ]", preset, this->buffer_[4], this->buffer_[5]);
      break;
    }
    default: {
      ESP_LOGV(TAG, "Unknown data: ");
      // log_buffer(this->buffer_, this->eot_index_);
      break;
    }
  }
}

void UpliftDeskComponent::reset_buffer_() {
  this->buffer_index_ = 0;
  this->eot_index_ = UPLIFT_DESK_BUFFER_LENGTH - 1;
}

void UpliftDeskComponent::send_cmd(const uint8_t cmd) {
  std::vector<uint8_t> args;
  this->send_cmd(cmd, args);
}

void UpliftDeskComponent::send_cmd(const uint8_t cmd, const std::vector<uint8_t> &args) {
  std::vector<uint8_t> data;
  data.push_back(0xF1);
  data.push_back(0xF1);
  data.push_back(cmd);
  data.push_back(args.size());
  data.insert(data.end(), args.begin(), args.end());

  uint8_t crc = cmd + args.size();
  for (auto arg : args) {
    crc += arg;
  }
  data.push_back(crc);

  data.push_back(0x7E);
  this->write_array(data);
}

void UpliftDeskComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Uplift Desk:");
  LOG_SENSOR("  ", "Height", this->height_sensor_);
  LOG_SENSOR("  ", "State", this->state_sensor_);
}

}  // namespace uplift_desk
}  // namespace esphome

