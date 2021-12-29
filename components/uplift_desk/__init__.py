import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@gabe565"]
DEPENDENCIES = ["uart"]

uplift_desk_ns = cg.esphome_ns.namespace("uplift_desk")
UpliftDeskComponent = uplift_desk_ns.class_(
    "UpliftDeskComponent", cg.Component, uart.UARTDevice
)

CONF_UPLIFT_DESK_ID = "uplift_desk_id"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(UpliftDeskComponent),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)

UPLIFT_DESK_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_UPLIFT_DESK_ID): cv.use_id(UpliftDeskComponent),
    }
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)
