#pragma once

#include "../uplift_desk.h"
#include "esphome/components/button/button.h"
#include "esphome/core/component.h"

namespace esphome {
namespace uplift_desk {

class UpliftDeskButton : public button::Button, public Component {
 public:
  void set_uplift_desk(UpliftDeskComponent *uplift_desk) { this->uplift_desk_ = uplift_desk; }
  void set_command(const uint8_t command) { this->command_ = command; }

  void dump_config() override;

 protected:
  void press_action() override;
  uint8_t command_;
  UpliftDeskComponent *uplift_desk_;
};

}  // namespace uplift_desk
}  // namespace esphome