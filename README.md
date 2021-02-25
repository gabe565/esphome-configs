# ESPHome Configs

A collection of my personal [ESPHome](https://esphome.io) configs and packages.


## Common

This is the common config that I include in all of my other configs. It includes:

- Secret-based Wifi configuration
- Wifi RSSI sensor
- API with password
- Logger
- OTA with password
- Device status binary sensor

### Setup

1. Copy `packages/` into your local ESPHome directory.
1. Open `secrets.yaml` and enter your Wifi connection info.


## Nitebird Light Strip

A configuration for Nitebird Light Strips. Configuration includes ADC mic config and has an audio sync mode which attempts to change brightness along with the ambient room sound.

### Setup

1. Copy the [common config](#common).
1. Copy `light_strip.yaml` and `packages/` into your local ESPHome directory.
1. Copy the relevant lines from `secrets.yaml` and fill in random API and OTA passwords.
1. Open `light_strip.yaml` and tweak the substitution values accordingly. The comments will explain each option.
1. Device will show up in ESPHome and be ready to build!
1. You will have to flash your device to Tasmota using [tuya-convert](https://github.com/ct-Open-Source/tuya-convert), then upload your ESPHome build on Tasmota's update page.


## OpenGarage

A configuration for [OpenGarage](https://opengarage.io) devices. Uses the official pin layout defined [here](https://github.com/OpenGarage/OpenGarage-Hardware/blob/master/Schematic/1.1/PINs.txt).

### Setup

1. Copy the [common config](#common).
1. Copy `garage_door.yaml` and `packages/` into your local ESPHome directory.
1. Copy the relevant lines from `secrets.yaml` and fill in random API and OTA passwords.
1. Open `garage_door.yaml` and tweak the substitution values accordingly. The comments will explain each option.
1. Device will show up in ESPHome and be ready to build!
