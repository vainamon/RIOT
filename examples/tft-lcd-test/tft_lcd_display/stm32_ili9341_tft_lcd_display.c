/*
 * stm32_ili9341_tft_lcd_display.c
 *
 *  Created on: 10 дек. 2019 г.
 *      Author: vainamon
 */

#include "stm32_ili9341_tft_lcd_display.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph_cpu_common.h"
#include "pm_layered.h"

#define ILI9341_TFTWIDTH  			320
#define ILI9341_TFTHEIGHT 			240

#define ILI9341_RESET				0x01
#define ILI9341_SLEEP_OUT			0x11
#define ILI9341_GAMMA				0x26
#define ILI9341_DISPLAY_OFF			0x28
#define ILI9341_DISPLAY_ON			0x29
#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_MAC					0x36
#define ILI9341_PIXEL_FORMAT		0x3A
#define ILI9341_WDB					0x51
#define ILI9341_WCD					0x53
#define ILI9341_RGB_INTERFACE		0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC					0xB6
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1				0xC5
#define ILI9341_VCOM2				0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA				0xE8
#define ILI9341_DTCB				0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC					0xF7

#define TFT_REG	 (*((volatile unsigned short *) 0x60000000))
#define TFT_DATA (*((volatile unsigned short *) 0x60080000))

#ifdef FSMC_USE_DMA
#ifndef TFT_LCD_DMA
#error "TFT_DMA not defined"
#endif
#ifndef TFT_LCD_DMA_CHAN
#error "TFT_DMA_CHAN not defined"
#endif

#define DMA_INC_NONE  (0x00)
#endif

static mutex_t tft_lock;

static inline void tft_send_command(uint8_t command){
	TFT_REG = command;
}

static inline void tft_send_data(uint16_t data){
	TFT_DATA = data;
}

static inline void tft_aquire(void){
	mutex_lock(&tft_lock);
#ifdef STM32_PM_STOP
	/* block STOP mode */
	pm_block(STM32_PM_STOP);
#endif
}

static inline void tft_release(void){
#ifdef STM32_PM_STOP
	/* unblock STOP mode */
	pm_unblock(STM32_PM_STOP);
#endif
	mutex_unlock(&tft_lock);
}

static inline void stm32_ili9341_gpio_init(void)
{
	// FSMC_D0
	gpio_init(GPIO_PIN(PORT_D, 14), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_D, 14), GPIO_AF12);
	// FSMC_D1
	gpio_init(GPIO_PIN(PORT_D, 15), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_D, 15), GPIO_AF12);
	// FSMC_D2
	gpio_init(GPIO_PIN(PORT_D, 0), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_D, 0), GPIO_AF12);
	// FSMC_D3
	gpio_init(GPIO_PIN(PORT_D, 1), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_D, 1), GPIO_AF12);
	// FSMC_D4
	gpio_init(GPIO_PIN(PORT_E, 7), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_E, 7), GPIO_AF12);
	// FSMC_D5
	gpio_init(GPIO_PIN(PORT_E, 8), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_E, 8), GPIO_AF12);
	// FSMC_D6
	gpio_init(GPIO_PIN(PORT_E, 9), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_E, 9), GPIO_AF12);
	// FSMC_D7
	gpio_init(GPIO_PIN(PORT_E, 10), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_E, 10), GPIO_AF12);
	// FSMC_D8
	gpio_init(GPIO_PIN(PORT_E, 11), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_E, 11), GPIO_AF12);
	// FSMC_D9
	gpio_init(GPIO_PIN(PORT_E, 12), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_E, 12), GPIO_AF12);
	// FSMC_D10
	gpio_init(GPIO_PIN(PORT_E, 13), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_E, 13), GPIO_AF12);
	// FSMC_D11
	gpio_init(GPIO_PIN(PORT_E, 14), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_E, 14), GPIO_AF12);
	// FSMC_D12
	gpio_init(GPIO_PIN(PORT_E, 15), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_E, 15), GPIO_AF12);
	// FSMC_D13
	gpio_init(GPIO_PIN(PORT_D, 8), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_D, 8), GPIO_AF12);
	// FSMC_D14
	gpio_init(GPIO_PIN(PORT_D, 9), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_D, 9), GPIO_AF12);
	// FSMC_D15
	gpio_init(GPIO_PIN(PORT_D, 10), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_D, 10), GPIO_AF12);

	// FSMC_A18
	gpio_init(GPIO_PIN(PORT_D, 13), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_D, 13), GPIO_AF12);
	// FSMC_NOE
	gpio_init(GPIO_PIN(PORT_D, 4), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_D, 4), GPIO_AF12);
	// FSMC_NWE
	gpio_init(GPIO_PIN(PORT_D, 5), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_D, 5), GPIO_AF12);
	// FSMC_NE1
	gpio_init(GPIO_PIN(PORT_D, 7), GPIO_IN);
	gpio_init_af(GPIO_PIN(PORT_D, 7), GPIO_AF12);
}

static inline void stm32_ili9341_fsmc_init(void)
{
	periph_clk_en(AHB3, RCC_AHB3ENR_FSMCEN);

	int FSMC_Bank = 0;

	FSMC_Bank1->BTCR[FSMC_Bank+1] = FSMC_BTR1_ADDSET_1 | FSMC_BTR1_DATAST_1;

	// Bank1 NOR/SRAM control register configuration
	FSMC_Bank1->BTCR[FSMC_Bank] = FSMC_BCR1_MWID_0 | FSMC_BCR1_WREN | FSMC_BCR1_MBKEN;
}

void stm32_ili9341_display_init(void)
{
	mutex_init(&tft_lock);

	tft_aquire();

	stm32_ili9341_gpio_init();
	stm32_ili9341_fsmc_init();

	xtimer_usleep(20000);

	tft_send_command(ILI9341_RESET);
	xtimer_usleep(10000);

	tft_send_command(ILI9341_POWERA);
	tft_send_data(0x39);
	tft_send_data(0x2C);
	tft_send_data(0x00);
	tft_send_data(0x34);
	tft_send_data(0x02);
	tft_send_command(ILI9341_POWERB);
	tft_send_data(0x00);
	tft_send_data(0xC1);
	tft_send_data(0x30);
	tft_send_command(ILI9341_DTCA);
	tft_send_data(0x85);
	tft_send_data(0x00);
	tft_send_data(0x78);
	tft_send_command(ILI9341_DTCB);
	tft_send_data(0x00);
	tft_send_data(0x00);
	tft_send_command(ILI9341_POWER_SEQ);
	tft_send_data(0x64);
	tft_send_data(0x03);
	tft_send_data(0x12);
	tft_send_data(0x81);
	tft_send_command(ILI9341_PRC);
	tft_send_data(0x20);
	tft_send_command(ILI9341_POWER1);
	tft_send_data(0x23);
	tft_send_command(ILI9341_POWER2);
	tft_send_data(0x10);
	tft_send_command(ILI9341_VCOM1);
	tft_send_data(0x3E);
	tft_send_data(0x28);
	tft_send_command(ILI9341_VCOM2);
	tft_send_data(0x86);
	tft_send_command(ILI9341_MAC);
	tft_send_data(0xE8);
	tft_send_command(ILI9341_PIXEL_FORMAT);
	tft_send_data(0x55); // 16-bit interface & 16-bit format
	tft_send_command(ILI9341_FRC);
	tft_send_data(0x00);
	tft_send_data(0x18);
	tft_send_command(ILI9341_DFC);
	tft_send_data(0x08);
	tft_send_data(0x82);
	tft_send_data(0x27);
	tft_send_command(0xF2);
	tft_send_data(0x00);
	tft_send_command(ILI9341_SLEEP_OUT);

	xtimer_usleep(10000);

	tft_send_command(ILI9341_DISPLAY_ON);

	tft_release();
}

static inline void stm32_ili9341_set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	tft_send_command(ILI9341_COLUMN_ADDR);
	tft_send_data(x0 >> 8);
	tft_send_data(x0 & 0xFF);
	tft_send_data(x1 >> 8);
	tft_send_data(x1 & 0xFF);

	tft_send_command(ILI9341_PAGE_ADDR);
	tft_send_data(y0>>8);
	tft_send_data(y0);
	tft_send_data(y1>>8);
	tft_send_data(y1);

	tft_send_command(ILI9341_GRAM);
}

#ifdef FSMC_USE_DMA
static inline void transfer_pixels_dma(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t *pixels, uint8_t flags)
{
	dma_acquire(TFT_LCD_DMA);

	uint32_t pixel_count = (x1 - x0 + 1) * (y1 - y0 + 1);
	uint16_t dma_index = 0;

	if(pixel_count > 0xFFFF){
		dma_index = 0xFFFF;
	} else {
		dma_index = pixel_count;
	}

	stm32_ili9341_set_addr_window(x0, y0, x1, y1);

	while (pixel_count > 0) {
		if(pixel_count > 0xFFFF){
			dma_index = 0xFFFF;
		} else {
			dma_index = pixel_count;
		}

		dma_transfer(TFT_LCD_DMA, TFT_LCD_DMA_CHAN,
				pixels, &TFT_DATA, dma_index, DMA_MEM_TO_MEM, flags | DMA_DATA_WIDTH_HALF_WORD);

		pixel_count -= dma_index;
	}

	dma_release(TFT_LCD_DMA);
}
#endif

void stm32_ili9341_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	if (x >= ILI9341_TFTWIDTH || y >= ILI9341_TFTHEIGHT)
		return;
	if (x + w - 1 >= ILI9341_TFTWIDTH)
		w = ILI9341_TFTWIDTH - x;
	if (y + h - 1 >= ILI9341_TFTHEIGHT)
		h = ILI9341_TFTHEIGHT - y;

	tft_aquire();

#ifdef FSMC_USE_DMA
	transfer_pixels_dma(x, y, x + w - 1, y + h - 1, &color, DMA_INC_NONE);
#else
	stm32_ili9341_set_addr_window(x, y, x + w - 1, y + h - 1);

	for (y = h; y > 0; y--)
		for (x = w; x > 0; x--)
			tft_send_data(color);
#endif

	tft_release();
}

void stm32_ili9341_fill_screen(uint16_t color)
{
	stm32_ili9341_fill_rect(0, 0, ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT, color);
}

static inline void stm32_ili9341_write_char(uint16_t x, uint16_t y, char ch, font_t* font, uint16_t color, uint16_t bgcolor)
{
	uint32_t i, b, j;

	stm32_ili9341_set_addr_window(x, y, x + font->width - 1, y + font->height - 1);

	for(i = 0; i < font->height; i++) {
		b = font->data[(ch - 32) * font->height + i];
		for(j = 0; j < font->width; j++) {
			if((b << j) & 0x8000)  {
				tft_send_data(color);
			} else {
				tft_send_data(bgcolor);
			}
		}
	}
}

void stm32_ili9341_write_string(uint16_t x, uint16_t y, const char* str, font_t* font, uint16_t color, uint16_t bgcolor)
{
	tft_aquire();

	while(*str) {
		if(x + font->width >= ILI9341_TFTWIDTH) {
			x = 0;
			y += font->height;

			if(y + font->height >= ILI9341_TFTHEIGHT) {
				break;
			}

			if(*str == ' ') {
				// skip spaces in the beginning of the new line
				str++;
				continue;
			}
		}

		stm32_ili9341_write_char(x, y, *str, font, color, bgcolor);

		x += font->width;
		str++;
	}

	tft_release();
}

static inline void stm32_ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
	if((x >= ILI9341_TFTWIDTH) || (y >= ILI9341_TFTHEIGHT))
		return;

	stm32_ili9341_set_addr_window(x, y, x+1, y+1);
	tft_send_data(color);
}

void stm32_ili9341_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color)
{
	int16_t dx, dy, sx, sy, err, e2;


	if (x0 > ILI9341_TFTWIDTH) {
		x0 = ILI9341_TFTWIDTH;
	}
	if (x1 > ILI9341_TFTWIDTH) {
		x1 = ILI9341_TFTWIDTH;
	}
	if (y0 > ILI9341_TFTHEIGHT) {
		y0 = ILI9341_TFTHEIGHT;
	}
	if (y1 > ILI9341_TFTHEIGHT) {
		y1 = ILI9341_TFTHEIGHT;
	}

	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = ((dx > dy) ? dx : -dy) / 2;

	tft_aquire();

	while (1) {
		if (x0 == x1 && y0 == y1) {
			break;
		}

		stm32_ili9341_draw_pixel(x0, y0, color);

		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}

	tft_release();
}
