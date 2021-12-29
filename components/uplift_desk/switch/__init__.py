from esphome.core import coroutine
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch, template
from esphome.const import (
    CONF_ENTITY_CATEGORY,
    CONF_ID,
    CONF_INVERTED,
    CONF_ICON,
    ENTITY_CATEGORY_CONFIG,
    ICON_POWER,
    CONF_COMMAND,
    CONF_SEND_EVERY,
    CONF_INTERLOCK,
)
from .. import uplift_desk_ns, UPLIFT_DESK_COMPONENT_SCHEMA, CONF_UPLIFT_DESK_ID

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

UPLIFT_DESK_SWITCH_SCHEMA = switch.SWITCH_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(UpliftDeskSwitch),
        cv.Optional(
            CONF_SEND_EVERY, default="1s"
        ): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_INVERTED): cv.invalid(
            "Uplift Desk switches do not support inverted mode!"
        ),
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
        cv.Optional(
            CONF_INTERLOCK_WAIT_TIME, default="1s"
        ): cv.positive_time_period_milliseconds,
    }
).extend(UPLIFT_DESK_COMPONENT_SCHEMA)


@coroutine
async def setup_conf(globalConfig, key, hub):
    if key in globalConfig:
        config = globalConfig[key]
        var = cg.new_Pvariable(config[CONF_ID])
        await cg.register_component(var, config)
        await switch.register_switch(var, config)

        cg.add(var.set_uplift_desk(hub))
        cg.add(var.set_command(config[CONF_COMMAND]))

        if CONF_SEND_EVERY in config:
            cg.add(var.set_send_every(config[CONF_SEND_EVERY]))

        return var


def to_code(config):
    hub = yield cg.get_variable(config[CONF_UPLIFT_DESK_ID])
    vars = {}
    for key in TYPES:
        vars[key] = yield setup_conf(config, key, hub)

    if not vars[CONF_UP] is None and not vars[CONF_DOWN] is None:
        cg.add(vars[CONF_UP].set_interlock([vars[CONF_DOWN]]))
        cg.add(vars[CONF_UP].set_interlock_wait_time(config[CONF_INTERLOCK_WAIT_TIME]))
        cg.add(vars[CONF_DOWN].set_interlock([vars[CONF_UP]]))
        cg.add(vars[CONF_DOWN].set_interlock_wait_time(config[CONF_INTERLOCK_WAIT_TIME]))
