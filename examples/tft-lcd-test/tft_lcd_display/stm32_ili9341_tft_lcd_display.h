/*
 * stm32_ili9341_tft_lcd_display.h
 *
 *  Created on: 10 дек. 2019 г.
 *      Author: vainamon
 */

#ifndef STM32_ILI9341_TFT_LCD_DISPLAY_H_
#define STM32_ILI9341_TFT_LCD_DISPLAY_H_

#include <stdint.h>
#include "fonts.h"

#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F

void stm32_ili9341_display_init(void);

void stm32_ili9341_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

void stm32_ili9341_fill_screen(uint16_t color);

void stm32_ili9341_write_string(uint16_t x, uint16_t y, const char* str, font_t* font, uint16_t color, uint16_t bgcolor);

void stm32_ili9341_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);

#endif /* STM32_ILI9341_TFT_LCD_DISPLAY_H_ */
