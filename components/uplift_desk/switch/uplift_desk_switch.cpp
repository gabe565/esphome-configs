#include "uplift_desk_switch.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace uplift_desk {

static const char *TAG = "uplift_desk.switch";

void UpliftDeskSwitch::loop() {
  if (this->state && this->send_every_) {
    const uint32_t now = millis();
    if (now - this->last_transmission_ > this->send_every_) {
      this->write_command_();
      this->last_transmission_ = now;
    }
  }
}

void UpliftDeskSwitch::write_command_() {
  ESP_LOGD(TAG, "'%s': Sending command...", this->get_name().c_str());
  this->uplift_desk_->send_cmd(this->command_);
}

void UpliftDeskSwitch::write_stop_() {
  ESP_LOGD(TAG, "'%s': Sending stop...", this->get_name().c_str());
  this->uplift_desk_->send_cmd_stop();
}

void UpliftDeskSwitch::write_state(bool state) {
  if (!state) {
    if (this->interlock_wait_time_ != 0) {
      // If we are switched off during the interlock wait time, cancel any pending
      // re-activations
      this->cancel_timeout("interlock");
    }

    this->publish_state(false);
    this->write_stop_();
    return;
  }

  // Turning ON, check interlocking
  bool found = false;
  for (auto *lock : this->interlock_) {
    if (lock == this)
      continue;

    if (lock->state) {
      lock->turn_off();
      found = true;
    }
  }
  if (found && this->interlock_wait_time_ != 0) {
    this->set_timeout("interlock", this->interlock_wait_time_, [this, state] {
      // Don't write directly, call the function again
      // (some other switch may have changed state while we were waiting)
      this->write_state(state);
    });
    return;
  }

  this->publish_state(true);
  this->write_command_();

  if (this->send_every_ == 0) {
    this->publish_state(false);
  } else {
    this->last_transmission_ = millis();
  }
}

void UpliftDeskSwitch::dump_config() {
  LOG_SWITCH("", "Uplift Desk Switch", this);
  if (this->send_every_) {
      ESP_LOGCONFIG(TAG, "  Send Every: %u", this->send_every_);
  }
  if (!this->interlock_.empty()) {
    ESP_LOGCONFIG(TAG, "  Interlocks:");
    for (auto *lock : this->interlock_) {
      if (lock == this)
        continue;
      ESP_LOGCONFIG(TAG, "    %s", lock->get_name().c_str());
    }
  }
}

}  // namespace uplift_desk
}  // namespace esphome