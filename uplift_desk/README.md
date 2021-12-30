# Uplift Desk

<img src="../../assets/uplift_desk/home-assistant.png?raw=true" width="50%">

A configuration for [Uplift Desk](https://www.upliftdesk.com/) desks. Uses the RJ12 port labeled `F`. This is the same port that the official (and unfortunately discontinued) Uplift Connect dongle uses.

## Supported Features

Currently, this integration supports the following:

- Commands
  - Move up
  - Move down
  - Stop
  - Save preset 1-4
  - Recall preset 1-4
  - Request an updated height reading (I decided to name it Sync)
  - Set a maximum height limit
  - Set a minimum height limit
  - Clear any height limit
- Sensors
  - Current height reading

## RJ12 Pinout

<img src="../../assets/uplift_desk/desk-pinout.jpg?raw=true" style="width: 25%">

I found the Uplift Connect dongle pinout via the [FCC filing](https://fccid.io/2ANKDJCP35NBLT/Internal-Photos/Internal-Photos-3727739) (Thanks to deadman96385 for sending that my way)

There are 6 PINs on the desk's RJ12 port. The first and sixth PIN are not needed. All communication occurs over UART and the ESP chip can be powered off of the 5v and GND PINs.
The port's pinout from left to right is as follows:

1. ? (Not known but not necessary)
1. GND
1. Tx
1. 5v
1. Rx
1. ? (Not known but not necessary)

## Hardware Setup

<img src="../../assets/uplift_desk/esp-wiring.jpg?raw=true" style="width: 25%">

My current hardware could use a little polish, but I mounted it under my desk and never have to see it so I don't mind for the moment!  
I use a NodeMCU in my setup. They can be powered off of 5v, so I directly power the ESP from the desk's 5v output.

## Software Setup

1. Copy the [common config](../common) into your local ESPHome configurations.
1. Copy `uplift_desk.yaml` and `packages/uplift_desk.yaml` into your local ESPHome configurations.
1. Copy the relevant lines from `secrets.yaml` and fill in random API and OTA passwords.
1. Open `uplift_desk.yaml` and tweak the substitution values accordingly. The comments will explain each option.
1. Device will show up in ESPHome and be ready to build!

## Planned Features

### Commands

I noticed a lot of the Uplift Desk UART commands correspond to the commands documented for a Jarvis desk in [phord/Jarvis](https://github.com/phord/Jarvis). I am still testing out some of these commands. Namely:

- Setting units (in, cm)
- Setting anti-collision sensitivity (high, medium, low)
- Reset
- Height calibration

I have implemented a "go to height" action locally, but it always ends up one or two inches off due to the speed of the desk. Slows down as it approaches a preset value. I would like to be able to do this when calling the "go to height" action, but I have not come across any indication that the desk supports speed values over UART. If it is not possible then I may be able to hone in the action by stopping it early, but so far it seems to be pretty inconsistent.

### Responses

- Sending a sync command makes the desk send its current height along with the saved heights of all 4 presets. I have not been able to decipher the preset heights. The values seem to be more precise than inches, with the second value as a decimal, but so far I have been unable to correlate them to actual height values or find any helpful calibration output. I will post a table of observed values below.

<details>
  <summary>Click to view observed prefix values</summary>

| Height | Response   |
|--------|------------|
| 25.3   | `20, 5`    |
| 25.3   | `20, 8`    |
| 25.3   | `20, 11`   |
| 25.3   | `20, 54`   |
| 28.3   | `25, 62`   |
| 28.3   | `25, 67`   |
| 28.3   | `25, 82`   |
| 35.1   | `37, 36`   |
| 37.8   | `41, 234`  |
| 37.9   | `42, 14`   |
| 39.9   | `45, 153`  |
| 39.9   | `45, 156`  |
| 40.0   | `45, 168`  |
| 42.4   | `50, 7`    |
| 42.8   | `50, 188`  |
| 42.9   | `50, 231`  |
| 43.0   | `51, 8`    |
| 43.1   | `51, 27`   |
| 43.2   | `51, 104`  |
| 50.8   | `64, 161`  |
| 50.8   | `64, 162`  |
</details>

## Sources

### Hardware

1.  ["2ANKDJCP35NBLT Bluetooth Box by ZHEJIANG JIECANG LINEAR MOTION TECHNOLOGY CO., LTD"](https://fccid.io/2ANKDJCP35NBLT). (2018, January 25). FCC ID. Retrieved January 19, 2021.
2.  [Jiecang Bluetooth Dongle Product Listing](https://en.jiecang.com/product/131.html). Retrieved January 19, 2021.

#### Images from deadman96385

1.  <https://imgur.com/a/MUbXwnM>
2.  <https://i.imgur.com/DyMf3Ee.jpg>
3.  <https://i.imgur.com/KtsWpVQ.jpg>
4.  <https://i.imgur.com/BS62C1E.jpg>
5.  <https://i.imgur.com/woWoQMe.jpg>
6.  <https://i.imgur.com/Lta5Nab.jpg>

### Software

1.  Justintout. (2020, April 16). GitHub - ["justintout/uplift-reconnect: A Flutter app to control Uplift desks with Uplift Connect BLE modules installed"](https://github.com/justintout/uplift-reconnect). GitHub. Retrieved January 19, 2021.
2.  Deadman96385. (2020, March 6). ["uplift_desk_controller_app/BluetoothHandler.java at a58bcadfb77ac993751758465f1cf20f71d6d8fd - deadman96385/uplift_desk_controller_app"](https://github.com/deadman96385/uplift_desk_controller_app/blob/a58bcadfb77ac993751758465f1cf20f71d6d8fd/app/src/main/java/com/deadman/uplift/BluetoothHandler.java). GitHub. Retrieved January 23, 2021.
3.  Phord. (2021, August 12). ["phord/Jarvis: Hacking the Jarvis standup desk from fully.com for home automation using an ESP8266 arduino interface"](https://github.com/phord/Jarvis). GitHub. Retrieved December 5, 2021.
4.  Ramot, Y. (2015, February 4). ["UpLift Desk wifi link"](https://hackaday.io/project/4173-uplift-desk-wifi-link). Hackaday.io.
5.  Horacek, L. (2019, April 14). ["Standing desk remote control"](https://hackaday.io/project/164931-standing-desk-remote-control). Hackaday.io.
6.  Hunleth, F. (2019, January 18). ["Nerves At Home: Controlling a Desk"](https://embedded-elixir.com/post/2019-01-18-nerves-at-home-desk-controller/). Embedded Elixir. Retrieved January 2021.
