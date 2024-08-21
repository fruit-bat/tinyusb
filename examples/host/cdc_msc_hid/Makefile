include ../../build_system/make/make.mk

INC += \
	src \
	$(TOP)/hw \

# Example source
EXAMPLE_SOURCE = \
  src/cdc_app.c \
  src/hid_app.c \
  src/hid_host_utils.c \
  src/hid_host_info.c \
  src/hid_host_joy.c \
  src/main.c \
  src/msc_app.c \

SRC_C += $(addprefix $(CURRENT_PATH)/, $(EXAMPLE_SOURCE))

# TinyUSB Host Stack source
# SRC_C += \
# 	src/class/cdc/cdc_host.c \
# 	src/class/hid/hid_host.c \
# 	src/class/hid/hid_rip.c \
# 	src/class/hid/hid_ri.c \
# 	src/class/msc/msc_host.c \
# 	src/host/hub.c \
# 	src/host/usbh.c \
# 	src/portable/ohci/ohci.c \
# 	src/portable/nxp/lpc17_40/hcd_lpc17_40.c

include ../../build_system/make/rules.mk
