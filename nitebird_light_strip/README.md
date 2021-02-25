# Nitebird Light Strip

A configuration for Nitebird Light Strips. Configuration includes ADC mic config and has an audio sync mode which attempts to change brightness along with the ambient room sound.

# Setup

1. Copy the [common config](../common) into your local ESPHome configurations.
1. Copy `light_strip.yaml` and `packages/` into your local ESPHome configurations.
1. Copy the relevant lines from `secrets.yaml` and fill in random API and OTA passwords.
1. Open `light_strip.yaml` and tweak the substitution values accordingly. The comments will explain each option.
1. Device will show up in ESPHome and be ready to build!
1. You will have to flash your device to Tasmota using [tuya-convert](https://github.com/ct-Open-Source/tuya-convert), then upload your ESPHome build on Tasmota's update page.
