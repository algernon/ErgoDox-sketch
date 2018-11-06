BOARD_HARDWARE_PATH ?= ./lib/hardware
KALEIDOSCOPE_PLUGIN_MAKEFILE_DIR ?= keyboardio/avr/build-tools/makefiles/
-include $(BOARD_HARDWARE_PATH)/$(KALEIDOSCOPE_PLUGIN_MAKEFILE_DIR)/rules.mk

setup:
	. ./.kaleidoscope-builder.conf && setup

.PHONY: setup
