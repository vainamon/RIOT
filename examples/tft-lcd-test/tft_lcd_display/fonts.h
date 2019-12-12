#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

typedef struct {
    const uint8_t 	width;
    uint8_t 		height;
    const uint16_t 	*data;
} font_t;


extern font_t FONT_7x10;
extern font_t FONT_11x18;
extern font_t FONT_16x26;

#endif // __FONTS_H__
