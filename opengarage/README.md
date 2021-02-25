# OpenGarage

A configuration for [OpenGarage](https://opengarage.io) devices. Uses the official pin layout defined [here](https://github.com/OpenGarage/OpenGarage-Hardware/blob/master/Schematic/1.1/PINs.txt).

## Setup

1. Copy the [common config](../common) into your local ESPHome configurations.
1. Copy `garage_door.yaml` and `packages/` into your local ESPHome configurations.
1. Copy the relevant lines from `secrets.yaml` and fill in random API and OTA passwords.
1. Open `garage_door.yaml` and tweak the substitution values accordingly. The comments will explain each option.
1. Device will show up in ESPHome and be ready to build!
