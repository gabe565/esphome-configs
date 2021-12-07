# Uplift Desk

A configuration for [Uplift Desk](https://www.upliftdesk.com/) desks. Uses the RJ12 port labeled "F". This is the same port that the official Uplift Connect dongle uses.

## RJ12 Pinout

I discovered the Uplift Connect dongle pinout via the [FCC filing](https://fccid.io/2ANKDJCP35NBLT/Internal-Photos/Internal-Photos-3727739).

The pinout based on the linked photos is as follows:
1. ?
1. Rx
1. 5v
1. Tx
1. GND
1. ?

## Setup

1. Copy the [common config](../common) into your local ESPHome configurations.
1. Copy `uplift_desk.yaml` and `packages/uplift_desk.yaml` into your local ESPHome configurations.
1. Copy the relevant lines from `secrets.yaml` and fill in random API and OTA passwords.
1. Open `uplift_desk.yaml` and tweak the substitution values accordingly. The comments will explain each option.
1. Device will show up in ESPHome and be ready to build!
