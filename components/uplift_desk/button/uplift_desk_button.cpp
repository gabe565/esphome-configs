#include "uplift_desk_button.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace uplift_desk {

static const char *TAG = "uplift_desk.button";

void UpliftDeskButton::press_action() {
    ESP_LOGD(TAG, "'%s': Sending command...", this->get_name().c_str());
  this->uplift_desk_->send_cmd(this->command_);
}

void UpliftDeskButton::dump_config() {
    LOG_BUTTON("", "Uplift Desk Button", this);
}

}  // namespace uplift_desk
}  // namespace esphome