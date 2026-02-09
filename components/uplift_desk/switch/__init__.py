import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import (
    CONF_COMMAND,
    CONF_ICON,
    CONF_ID,
    CONF_INVERTED,
    CONF_SEND_EVERY,
)

from .. import CONF_UPLIFT_DESK_ID, UPLIFT_DESK_COMPONENT_SCHEMA, uplift_desk_ns

AUTO_LOAD = ["uplift_desk"]

UpliftDeskSwitch = uplift_desk_ns.class_("UpliftDeskSwitch", cg.Component)

CONF_UP = "up"
CONF_DOWN = "down"
CONF_INTERLOCK_WAIT_TIME = "interlock_wait_time"

ICON_UP = "mdi:arrow-up"
ICON_DOWN = "mdi:arrow-down"

COMMAND_UP = 0x01
COMMAND_DOWN = 0x02

TYPES = [
    CONF_UP,
    CONF_DOWN,
]

UPLIFT_DESK_SWITCH_SCHEMA = switch.switch_schema(UpliftDeskSwitch).extend(
    {
        cv.Optional(CONF_SEND_EVERY, default="1s"): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_INVERTED): cv.invalid("Uplift Desk switches do not support inverted mode!"),
    }
)


CONFIG_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_UP): UPLIFT_DESK_SWITCH_SCHEMA.extend(
            {
                cv.Optional(CONF_ICON, default=ICON_UP): cv.icon,
                cv.Optional(CONF_COMMAND, default=COMMAND_UP): cv.hex_int,
            }
        ),
        cv.Optional(CONF_DOWN): UPLIFT_DESK_SWITCH_SCHEMA.extend(
            {
                cv.Optional(CONF_ICON, default=ICON_DOWN): cv.icon,
                cv.Optional(CONF_COMMAND, default=COMMAND_DOWN): cv.hex_int,
            }
        ),
        cv.Optional(CONF_INTERLOCK_WAIT_TIME, default="1s"): cv.positive_time_period_milliseconds,
    }
).extend(UPLIFT_DESK_COMPONENT_SCHEMA)


async def setup_conf(config, key, hub):
    if key in config:
        conf = config[key]
        var = cg.new_Pvariable(conf[CONF_ID])
        await cg.register_component(var, conf)
        await switch.register_switch(var, conf)

        cg.add(var.set_uplift_desk(hub))
        cg.add(var.set_command(conf[CONF_COMMAND]))

        if CONF_SEND_EVERY in conf:
            cg.add(var.set_send_every(conf[CONF_SEND_EVERY]))

        return var
    return None


async def to_code(config):
    hub = await cg.get_variable(config[CONF_UPLIFT_DESK_ID])
    switches = {}
    for key in TYPES:
        switches[key] = await setup_conf(config, key, hub)

    if switches[CONF_UP] is not None and switches[CONF_DOWN] is not None:
        cg.add(switches[CONF_UP].set_interlock([switches[CONF_DOWN]]))
        cg.add(switches[CONF_UP].set_interlock_wait_time(config[CONF_INTERLOCK_WAIT_TIME]))
        cg.add(switches[CONF_DOWN].set_interlock([switches[CONF_UP]]))
        cg.add(switches[CONF_DOWN].set_interlock_wait_time(config[CONF_INTERLOCK_WAIT_TIME]))
