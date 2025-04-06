//
// Created by sergey on 4/2/25.
//

#ifndef LED_COM_LEDSERVICE_H
#define LED_COM_LEDSERVICE_H

#include <driver/rmt_tx.h>
#include "services/led/encoder/led_strip_encoder.h"

#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define RMT_LED_STRIP_GPIO_NUM      GPIO_NUM_10
#define LED_COUNT         16

class LedService {
public:
    void init();
    void startLoop();
private:
    rmt_channel_handle_t led_chan = nullptr;
    rmt_encoder_handle_t led_encoder = nullptr;
    rmt_transmit_config_t tx_config;
    uint8_t* led_strip_pixels;
};


#endif //LED_COM_LEDSERVICE_H
