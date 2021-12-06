#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace uplift_desk {

const extern uint8_t STATE_IDLE;
const extern uint8_t STATE_MOVING_UP;
const extern uint8_t STATE_MOVING_DOWN;

const extern uint8_t UPLIFT_DESK_BUFFER_LENGTH;
const extern uint8_t UPLIFT_DESK_CMD_LENGTH;

const extern uint8_t UPLIFT_DESK_UP;
const extern uint8_t UPLIFT_DESK_DOWN;
const extern uint8_t UPLIFT_DESK_STOP;
const extern uint8_t UPLIFT_DESK_SAVE_SITTING;
const extern uint8_t UPLIFT_DESK_SAVE_STANDING;
const extern uint8_t UPLIFT_DESK_RECALL_SITTING;
const extern uint8_t UPLIFT_DESK_RECALL_STANDING;
const extern uint8_t UPLIFT_DESK_SYNC;

class UpliftDeskComponent : public Component, public uart::UARTDevice {
 public:
  UpliftDeskComponent();

  void setup() override { this->send_cmd_sync(); };
  void loop() override;
  void dump_config() override;

  void set_height_sensor(sensor::Sensor *height_sensor) { this->height_sensor_ = height_sensor; }
  sensor::Sensor *get_height_sensor() { return this->height_sensor_; }
  void set_state_sensor(sensor::Sensor *state_sensor) { this->state_sensor_ = state_sensor; }
  sensor::Sensor *get_state_sensor() { return this->state_sensor_; }

  void send_cmd_up() { this->send_cmd_(UPLIFT_DESK_UP); }
  void send_cmd_down() { this->send_cmd_(UPLIFT_DESK_DOWN); }
  void send_cmd_stop() { this->send_cmd_(UPLIFT_DESK_STOP); }
  void send_cmd_save_sit() { this->send_cmd_(UPLIFT_DESK_SAVE_SITTING); }
  void send_cmd_save_stand() { this->send_cmd_(UPLIFT_DESK_SAVE_STANDING); }
  void send_cmd_sit() { this->send_cmd_(UPLIFT_DESK_RECALL_SITTING); }
  void send_cmd_stand() { this->send_cmd_(UPLIFT_DESK_RECALL_STANDING); }
  void send_cmd_sync() { this->send_cmd_(UPLIFT_DESK_SYNC); }

 protected:
  bool check_byte_();
  void parse_data_();
  void send_cmd_(const uint8_t cmd);
  void reset_buffer_();

  uint8_t buffer_[16];
  uint8_t buffer_index_{0};
  uint8_t eot_index_{15};
  uint32_t last_transmission_{0};
  sensor::Sensor *height_sensor_{nullptr};
  sensor::Sensor *state_sensor_{nullptr};
};

} // namespace uplift_desk
} // namespace esphome

