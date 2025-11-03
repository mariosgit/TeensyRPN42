#!/bin/bash

# PlatformIO wrapper script to use the correct Python virtual environment
# This fixes the PlatformIO terminal issue for TeensyRPN42 project

exec /home/mario/.platformio/penv/bin/pio "$@"
