APPLICATION = tft-lcd-test
RIOTBASE ?= $(CURDIR)/../..
BOARD ?= native
QUIET ?= 1

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

USEMODULE += xtimer
USEMODULE += periph_dma
USEMODULE += stdio_null

TFT_LCD_DMA ?= 		\(3\)
TFT_LCD_DMA_CHAN ?= \(0\)

CFLAGS += -DUART_USE_DMA  
CFLAGS += -DSPI_USE_DMA
CFLAGS += -DFSMC_USE_DMA

CFLAGS += -DTFT_LCD_DMA=$(TFT_LCD_DMA) -DTFT_LCD_DMA_CHAN=$(TFT_LCD_DMA_CHAN)

USEMODULE += tft_lcd_display
DIRS += tft_lcd_display

include $(RIOTBASE)/Makefile.include
