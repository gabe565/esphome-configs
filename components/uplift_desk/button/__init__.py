import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import (
    CONF_COMMAND,
    CONF_ID,
    ENTITY_CATEGORY_CONFIG,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

from .. import CONF_UPLIFT_DESK_ID, UPLIFT_DESK_COMPONENT_SCHEMA, uplift_desk_ns
from .const import *  # noqa: F403

AUTO_LOAD = ["uplift_desk"]

UpliftDeskButton = uplift_desk_ns.class_("UpliftDeskButton", cg.Component)

TYPES = [
    CONF_STOP,  # noqa: F405
    CONF_SAVE_PRESET_1,  # noqa: F405
    CONF_SAVE_PRESET_2,  # noqa: F405
    CONF_SAVE_PRESET_3,  # noqa: F405
    CONF_SAVE_PRESET_4,  # noqa: F405
    CONF_PRESET_1,  # noqa: F405
    CONF_PRESET_2,  # noqa: F405
    CONF_PRESET_3,  # noqa: F405
    CONF_PRESET_4,  # noqa: F405
    CONF_SYNC,  # noqa: F405
    CONF_LIMIT_SET_MIN,  # noqa: F405
    CONF_LIMIT_SET_MAX,  # noqa: F405
    CONF_LIMIT_CLEAR,  # noqa: F405
]


def uplift_desk_button_schema(
    command,
    icon: str = None,
    entity_category: str = None,
    device_class: str = None,
):
    # Build kwargs dict conditionally to avoid passing None values
    kwargs = {"class_": UpliftDeskButton}
    if icon is not None:
        kwargs["icon"] = icon
    if entity_category is not None:
        kwargs["entity_category"] = entity_category
    if device_class is not None:
        kwargs["device_class"] = device_class

    return button.button_schema(**kwargs).extend(
        {
            cv.GenerateID(): cv.declare_id(UpliftDeskButton),
            cv.Optional(CONF_COMMAND, default=command): cv.hex_int,
        }
    )


CONFIG_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_STOP): uplift_desk_button_schema(  # noqa: F405
            COMMAND_STOP,  # noqa: F405
            icon=ICON_STOP,  # noqa: F405
        ),
        cv.Optional(CONF_SAVE_PRESET_1): uplift_desk_button_schema(  # noqa: F405
            COMMAND_SAVE_PRESET_1,  # noqa: F405
            icon=ICON_FLOPPY,  # noqa: F405
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_SAVE_PRESET_2): uplift_desk_button_schema(  # noqa: F405
            COMMAND_SAVE_PRESET_2,  # noqa: F405
            icon=ICON_FLOPPY,  # noqa: F405
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_SAVE_PRESET_3): uplift_desk_button_schema(  # noqa: F405
            COMMAND_SAVE_PRESET_3,  # noqa: F405
            icon=ICON_FLOPPY,  # noqa: F405
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_SAVE_PRESET_4): uplift_desk_button_schema(  # noqa: F405
            COMMAND_SAVE_PRESET_4,  # noqa: F405
            icon=ICON_FLOPPY,  # noqa: F405
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_PRESET_1): uplift_desk_button_schema(  # noqa: F405
            COMMAND_PRESET_1,  # noqa: F405
            icon=ICON_NUMERIC_1_BOX,  # noqa: F405
        ),
        cv.Optional(CONF_PRESET_2): uplift_desk_button_schema(  # noqa: F405
            COMMAND_PRESET_2,  # noqa: F405
            icon=ICON_NUMERIC_2_BOX,  # noqa: F405
        ),
        cv.Optional(CONF_PRESET_3): uplift_desk_button_schema(  # noqa: F405
            COMMAND_PRESET_3,  # noqa: F405
            icon=ICON_NUMERIC_3_BOX,  # noqa: F405
        ),
        cv.Optional(CONF_PRESET_4): uplift_desk_button_schema(  # noqa: F405
            COMMAND_PRESET_4,  # noqa: F405
            icon=ICON_NUMERIC_4_BOX,  # noqa: F405
        ),
        cv.Optional(CONF_SYNC): uplift_desk_button_schema(  # noqa: F405
            COMMAND_SYNC,  # noqa: F405
            icon=ICON_SYNC,  # noqa: F405
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_LIMIT_SET_MIN): uplift_desk_button_schema(  # noqa: F405
            COMMAND_LIMIT_SET_MIN,  # noqa: F405
            icon=ICON_FORMAT_VERTICAL_ALIGN_BOTTOM,  # noqa: F405
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_LIMIT_SET_MAX): uplift_desk_button_schema(  # noqa: F405
            COMMAND_LIMIT_SET_MAX,  # noqa: F405
            icon=ICON_FORMAT_VERTICAL_ALIGN_TOP,  # noqa: F405
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_LIMIT_CLEAR): uplift_desk_button_schema(  # noqa: F405
            COMMAND_LIMIT_CLEAR,  # noqa: F405
            icon=ICON_CLOSE_BOX,  # noqa: F405
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
    }
).extend(UPLIFT_DESK_COMPONENT_SCHEMA)


async def setup_conf(config, key, hub):
    if key in config:
        conf = config[key]
        var = cg.new_Pvariable(conf[CONF_ID])
        await cg.register_component(var, conf)
        await button.register_button(var, conf)

        cg.add(var.set_uplift_desk(hub))
        cg.add(var.set_command(conf[CONF_COMMAND]))


async def to_code(config):
    hub = await cg.get_variable(config[CONF_UPLIFT_DESK_ID])
    for key in TYPES:
        await setup_conf(config, key, hub)
