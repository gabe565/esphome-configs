from esphome.core import coroutine
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, DEVICE_CLASS_EMPTY
from . import UpliftDeskComponent

DEPENDENCIES = ["uplift_desk"]

CONF_HEIGHT = "height"
CONF_STATE = "state"

ICON_ARROW_EXPAND_VERTICAL = "mdi:arrow-expand-vertical"

TYPES = {
    CONF_HEIGHT: "set_height_sensor",
    CONF_STATE: "set_state_sensor",
}

UNIT_INCHES = "in"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(UpliftDeskComponent),
        cv.Optional(CONF_HEIGHT): sensor.sensor_schema(
            UNIT_INCHES, ICON_ARROW_EXPAND_VERTICAL, 1, DEVICE_CLASS_EMPTY
        ),
        cv.Optional(CONF_STATE): sensor.sensor_schema(
            cv.UNDEFINED, cv.UNDEFINED, 0, DEVICE_CLASS_EMPTY
        ),
    }
)


@coroutine
def setup_conf(config, key, hub, func_name):
    if key in config:
        conf = config[key]
        var = yield sensor.new_sensor(conf)
        func = getattr(hub, func_name)
        cg.add(func(var))


def to_code(config):
    hub = yield cg.get_variable(config[CONF_ID])
    for key, funcName in TYPES.items():
        yield setup_conf(config, key, hub, funcName)
