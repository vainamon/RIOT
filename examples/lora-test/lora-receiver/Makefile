APPLICATION = lora_receiver_test
RIOTBASE ?= $(CURDIR)/../../..
BOARD ?= stm32f4xx
QUIET ?= 1

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

USEMODULE += od
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += xtimer
USEMODULE += periph_dma

DRIVER ?= sx1276

# use SX1276 by default
USEMODULE += $(DRIVER)

CFLAGS += -DUART_USE_DMA
CFLAGS += -DSPI_USE_DMA

include $(RIOTBASE)/Makefile.include
