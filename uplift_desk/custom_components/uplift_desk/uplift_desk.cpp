#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"
#include "uplift_desk.h"

namespace esphome {
namespace uplift_desk {

static const char *TAG = "uplift_desk";

const uint8_t STATE_IDLE = 0;
const uint8_t STATE_MOVING_UP = 1;
const uint8_t STATE_MOVING_DOWN = 2;

const uint8_t UPLIFT_DESK_BUFFER_LENGTH = 16;
const uint8_t UPLIFT_DESK_CMD_LENGTH = 6;

const uint8_t UPLIFT_DESK_UP = 0x01;
const uint8_t UPLIFT_DESK_DOWN = 0x02;
const uint8_t UPLIFT_DESK_STOP = 0x2B;
const uint8_t UPLIFT_DESK_SAVE_SITTING = 0x03;
const uint8_t UPLIFT_DESK_SAVE_STANDING = 0x04;
const uint8_t UPLIFT_DESK_RECALL_SITTING = 0x05;
const uint8_t UPLIFT_DESK_RECALL_STANDING = 0x06;
const uint8_t UPLIFT_DESK_SYNC = 0x07;

UpliftDeskComponent::UpliftDeskComponent() : uart::UARTDevice() {};

void UpliftDeskComponent::loop() {
  while (this->available()) {
    this->read_byte(&this->buffer_[this->buffer_index_]);
    if (!this->check_byte_()) {
      this->reset_buffer_();
      this->status_set_warning();
    }

    if (this->buffer_index_ == this->last_index_) {
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
  } else if (index == 3) {
    this->last_index_ = index + byte + 2;  // Add 2 due to checksum and end byte
    return true;
  } else if (index == this->last_index_ - 1) {
    const uint8_t checksum_index = this->last_index_ - 1;
    uint8_t checksum = 0;
    for (uint8_t i = 2; i < checksum_index; i++) {
      checksum += this->buffer_[i];
    }
    if (checksum != this->buffer_[checksum_index]) {
      ESP_LOGW(TAG, "Invalid checksum: 0x%02X != 0x%02X", checksum, this->buffer_[checksum_index]);
      return false;
    }
    return true;
  } else if (index == this->last_index_) {
    return byte == 0x7E;
  }

  return true;
}

void UpliftDeskComponent::parse_data_() {
  switch (this->buffer_[2]) {
    case 0x01: {
      float inches = ((this->buffer_[4] << 8) | this->buffer_[5]) / 10.0;
      ESP_LOGV(TAG, "Read height: [ %d, %d ], %.1f\"", this->buffer_[4], this->buffer_[5], inches);

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
    default: {
      ESP_LOGV(TAG, "Unknown data: ");
      for (uint8_t i = 0; i <= this->last_index_; i++) {
        ESP_LOGV(TAG, "  i=%u: 0b" BYTE_TO_BINARY_PATTERN " (0x%02X, %i)", i, BYTE_TO_BINARY(this->buffer_[i]),
                 this->buffer_[i], this->buffer_[i]);
      }
      break;
    }
  }
}

void UpliftDeskComponent::reset_buffer_() {
  this->buffer_index_ = 0;
  this->last_index_ = UPLIFT_DESK_BUFFER_LENGTH;
}

void UpliftDeskComponent::send_cmd_(const uint8_t cmd) {
  uint8_t data[] = {0xF1, 0xF1, cmd, 0x00, 0x00, 0x7E};
  uint8_t checksum = 0;

  for (uint8_t i = 2; i < 4; i++) {
    checksum += data[i];
  }
  data[4] = checksum;

  this->write_array(data, UPLIFT_DESK_CMD_LENGTH);
}

void UpliftDeskComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Uplift Desk:");
  LOG_SENSOR("  ", "Height", this->height_sensor_);
  LOG_SENSOR("  ", "State", this->state_sensor_);
}

}  // namespace uplift_desk
}  // namespace esphome

