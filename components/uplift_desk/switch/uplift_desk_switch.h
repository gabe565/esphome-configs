#pragma once

#include "../uplift_desk.h"
#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"

namespace esphome {
namespace uplift_desk {

class UpliftDeskSwitch : public switch_::Switch, public Component {
 public:
  void loop() override;

  void set_uplift_desk(UpliftDeskComponent *uplift_desk) { this->uplift_desk_ = uplift_desk; }
  void set_command(const uint8_t command) { this->command_ = command; }
  void set_send_every(const uint32_t send_every) { this->send_every_ = send_every; }
  void set_interlock(const std::vector<UpliftDeskSwitch *> &interlock) { this->interlock_ = interlock; }
  void set_interlock_wait_time(uint32_t interlock_wait_time) { interlock_wait_time_ = interlock_wait_time; }

  void dump_config() override;

 protected:
  void write_command_();
  void write_stop_();
  void write_state(bool state) override;
  uint8_t command_;
  uint32_t send_every_;
  uint32_t last_transmission_;
  UpliftDeskComponent *uplift_desk_;
  std::vector<UpliftDeskSwitch *> interlock_;
  uint32_t interlock_wait_time_{0};
};

}  // namespace uplift_desk
}  // namespace esphome