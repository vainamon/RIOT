#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "tft_lcd_display/stm32_ili9341_tft_lcd_display.h"

/* set interval to 1 second */
#define INTERVAL (1U * US_PER_SEC)

int main(void)
{
	stm32_ili9341_display_init();

	stm32_ili9341_fill_screen(ILI9341_GREENYELLOW);

	xtimer_ticks32_t last_wakeup = xtimer_now();

	int i = 0;

	while(1) {
		xtimer_periodic_wakeup(&last_wakeup, INTERVAL);

		uint16_t color = ILI9341_BLACK;
		uint16_t line_color = (i / 4) % 2 ? ILI9341_WHITE : ILI9341_BLACK;

		switch(i % 4) {
		case 0:
			if ((i / 4) % 2)
				color = ILI9341_BLUE;
			else
				color = ILI9341_RED;
			break;
		case 1:
			if ((i / 4) % 2)
				color = ILI9341_GREEN;
			else
				color = ILI9341_YELLOW;
			break;
		case 2:
			if ((i / 4) % 2)
				color = ILI9341_ORANGE;
			else
				color = ILI9341_OLIVE;
			break;
		case 3:
			if ((i / 4) % 2)
				color = ILI9341_MAGENTA;
			else
				color = ILI9341_PURPLE;
			break;
		default:
			color = ILI9341_WHITE;
			break;
		}

		stm32_ili9341_fill_rect(((i % 4) * 80), 0, 80, 240, color);

		stm32_ili9341_write_string(((i % 4) * 80) + 10, 110, "Hello!", &FONT_11x18, ILI9341_DARKCYAN, color);

		stm32_ili9341_draw_line(((i % 4) * 80), 128, ((i % 4) * 80) + 80, 128, line_color);

		if ((i / 4) % 2)
			LED0_TOGGLE;
		else
			LED1_TOGGLE;

		i++;
	}

	return 0;
}
