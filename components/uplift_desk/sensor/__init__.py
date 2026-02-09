import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor

from .. import CONF_UPLIFT_DESK_ID, UPLIFT_DESK_COMPONENT_SCHEMA

AUTO_LOAD = ["uplift_desk"]

CONF_HEIGHT = "height"
CONF_STATE = "state"

ICON_ARROW_EXPAND_VERTICAL = "mdi:arrow-expand-vertical"

UNIT_INCHES = "in"

TYPES = {
    CONF_HEIGHT: "set_height_sensor",
    CONF_STATE: "set_state_sensor",
}


CONFIG_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_HEIGHT): sensor.sensor_schema(
            unit_of_measurement=UNIT_INCHES,
            icon=ICON_ARROW_EXPAND_VERTICAL,
            accuracy_decimals=1,
        ),
        cv.Optional(CONF_STATE): sensor.sensor_schema(
            accuracy_decimals=0,
        ),
    }
).extend(UPLIFT_DESK_COMPONENT_SCHEMA)


async def setup_conf(config, key, hub, func_name):
    if key in config:
        conf = config[key]
        var = await sensor.new_sensor(conf)
        func = getattr(hub, func_name)
        cg.add(func(var))


async def to_code(config):
    hub = await cg.get_variable(config[CONF_UPLIFT_DESK_ID])
    for key, func_name in TYPES.items():
        await setup_conf(config, key, hub, func_name)
