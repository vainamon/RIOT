APPLICATION = esp01s_at_client
RIOTBASE ?= $(CURDIR)/../../..
BOARD ?= stm32f4xx
QUIET ?= 1

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

USEMODULE += at
USEMODULE += periph_dma

CFLAGS += -DUART_USE_DMA
CFLAGS += -DSPI_USE_DMA
CFLAGS += -DAT_PRINT_INCOMING=0 -DAT_SEND_ECHO=0 -DAT_SEND_EOL=\"\\r\\n\"

include $(RIOTBASE)/Makefile.include
