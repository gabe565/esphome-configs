#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace uplift_desk {

const extern uint8_t STATE_IDLE;
const extern uint8_t STATE_MOVING_UP;
const extern uint8_t STATE_MOVING_DOWN;

#define UPLIFT_DESK_BUFFER_LENGTH 16
#define UPLIFT_DESK_CMD_LENGTH 6

#define UPLIFT_DESK_UP 0x01
#define UPLIFT_DESK_DOWN 0x02
#define UPLIFT_DESK_STOP 0x2B
#define UPLIFT_DESK_SAVE_PRESET_1 0x03
#define UPLIFT_DESK_SAVE_PRESET_2 0x04
#define UPLIFT_DESK_SAVE_PRESET_3 0x25
#define UPLIFT_DESK_SAVE_PRESET_4 0x26
#define UPLIFT_DESK_PRESET_1 0x05
#define UPLIFT_DESK_PRESET_2 0x06
#define UPLIFT_DESK_PRESET_3 0x27
#define UPLIFT_DESK_PRESET_4 0x28
#define UPLIFT_DESK_SYNC 0x07
#define UPLIFT_DESK_LIMIT_SET_MIN 0x22
#define UPLIFT_DESK_LIMIT_SET_MAX 0x21
#define UPLIFT_DESK_LIMIT_CLEAR 0x23

class UpliftDeskComponent : public Component, public uart::UARTDevice {
 public:
  UpliftDeskComponent();

  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_height_sensor(sensor::Sensor *height_sensor) { this->height_sensor_ = height_sensor; }
  sensor::Sensor *get_height_sensor() { return this->height_sensor_; }
  void set_state_sensor(sensor::Sensor *state_sensor) { this->state_sensor_ = state_sensor; }
  sensor::Sensor *get_state_sensor() { return this->state_sensor_; }

  void send_cmd(const uint8_t cmd);
  void send_cmd(const uint8_t cmd, const std::vector<uint8_t> &args);
  void send_cmd_stop() { this->send_cmd(UPLIFT_DESK_STOP); }
  void send_cmd_save_preset_1() { this->send_cmd(UPLIFT_DESK_SAVE_PRESET_1); }
  void send_cmd_save_preset_2() { this->send_cmd(UPLIFT_DESK_SAVE_PRESET_2); }
  void send_cmd_save_preset_3() { this->send_cmd(UPLIFT_DESK_SAVE_PRESET_3); }
  void send_cmd_save_preset_4() { this->send_cmd(UPLIFT_DESK_SAVE_PRESET_4); }
  void send_cmd_preset_1() { this->send_cmd(UPLIFT_DESK_PRESET_1); }
  void send_cmd_preset_2() { this->send_cmd(UPLIFT_DESK_PRESET_2); }
  void send_cmd_preset_3() { this->send_cmd(UPLIFT_DESK_PRESET_3); }
  void send_cmd_preset_4() { this->send_cmd(UPLIFT_DESK_PRESET_4); }
  void send_cmd_sync() { this->send_cmd(UPLIFT_DESK_SYNC); }
  void send_cmd_limit_set_min() { this->send_cmd(UPLIFT_DESK_LIMIT_SET_MIN); }
  void send_cmd_limit_set_max() { this->send_cmd(UPLIFT_DESK_LIMIT_SET_MAX); }
  void send_cmd_limit_clear() { this->send_cmd(UPLIFT_DESK_LIMIT_CLEAR); }

 protected:
  bool check_byte_();
  void parse_data_();
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

