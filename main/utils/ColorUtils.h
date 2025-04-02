//
// Created by sergey on 4/2/25.
//

#ifndef LED_COM_COLORUTILS_H
#define LED_COM_COLORUTILS_H

#include <stdint.h>

class ColorUtils {
public:
    /**
    * @brief Simple helper function, converting HSV color space to RGB color space
    *
    * Wiki: https://en.wikipedia.org/wiki/HSL_and_HSV
    *
    */
    static void hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b);
};


#endif //LED_COM_COLORUTILS_H
