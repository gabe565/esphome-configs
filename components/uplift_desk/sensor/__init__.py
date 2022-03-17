from esphome.core import coroutine
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, DEVICE_CLASS_EMPTY
from .. import uplift_desk_ns, UPLIFT_DESK_COMPONENT_SCHEMA, CONF_UPLIFT_DESK_ID

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
            device_class=DEVICE_CLASS_EMPTY
        ),
        cv.Optional(CONF_STATE): sensor.sensor_schema(
            unit_of_measurement=cv.UNDEFINED,
            icon=cv.UNDEFINED,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY
        ),
    }
).extend(UPLIFT_DESK_COMPONENT_SCHEMA)


@coroutine
def setup_conf(globalConfig, key, hub, func_name):
    if key in globalConfig:
        config = globalConfig[key]
        var = yield sensor.new_sensor(config)
        func = getattr(hub, func_name)
        cg.add(func(var))


def to_code(config):
    hub = yield cg.get_variable(config[CONF_UPLIFT_DESK_ID])
    for key, funcName in TYPES.items():
        yield setup_conf(config, key, hub, funcName)
