APPLICATION = mpu_test
RIOTBASE ?= $(CURDIR)/../..
BOARD ?= native
QUIET ?= 1

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

USEMODULE += xtimer
USEMODULE += periph_dma
USEMODULE += mpu9250

CFLAGS += -DUART_USE_DMA
CFLAGS += -DSPI_USE_DMA

include $(RIOTBASE)/Makefile.include
