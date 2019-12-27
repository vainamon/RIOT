APPLICATION = nrf24l01p_sender
RIOTBASE ?= $(CURDIR)/../../..
BOARD ?= stm32f4xx
QUIET ?= 1

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

USEMODULE += xtimer
USEMODULE += nrf24l01p
USEMODULE += periph_dma

# set default device parameters in case they are undefined
SPI_PORT ?= SPI_DEV\(0\)
CE_PIN   ?= GPIO_PIN\(PORT_B,6\)
CS_PIN   ?= GPIO_PIN\(PORT_B,7\)
IRQ_PIN  ?= GPIO_PIN\(PORT_B,8\)

# export parameters
CFLAGS += -DSPI_PORT=$(SPI_PORT)
CFLAGS += -DCE_PIN=$(CE_PIN)
CFLAGS += -DCS_PIN=$(CS_PIN)
CFLAGS += -DIRQ_PIN=$(IRQ_PIN)

CFLAGS += -DUART_USE_DMA
CFLAGS += -DSPI_USE_DMA

include $(RIOTBASE)/Makefile.include
