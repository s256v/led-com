//
// Created by sergey on 4/2/25.
//

#include "LedService.h"
#include <cstring>
#include "driver/rmt_tx.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "utils/ColorUtils.h"

static const char *CLASS_TAG = "LedService";

void LedService::init() {
    ESP_LOGI(CLASS_TAG, "Create RMT TX channel");
    rmt_tx_channel_config_t tx_chan_config = {
            .gpio_num = RMT_LED_STRIP_GPIO_NUM,
            .clk_src = RMT_CLK_SRC_DEFAULT, // select source clock
            .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
            .mem_block_symbols = 64, // increase the block size can make the LED less flickering
            .trans_queue_depth = 4, // set the number of transactions that can be pending in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));

    ESP_LOGI(CLASS_TAG, "Install led strip encoder");

    led_strip_encoder_config_t encoder_config = {
            .resolution = RMT_LED_STRIP_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &this->led_encoder));

    ESP_LOGI(CLASS_TAG, "Enable RMT TX channel");
    ESP_ERROR_CHECK(rmt_enable(this->led_chan));

    ESP_LOGI(CLASS_TAG, "Start LED rainbow chase");
    this->tx_config = {
            .loop_count = 0, // no transfer loop
    };
    this->led_strip_pixels = static_cast<uint8_t *>(malloc(LED_COUNT * 30));
}

void LedService::startLoop() {
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint16_t hue = 0;
    uint16_t start_rgb = 0;

    uint8_t delayMS = 10;

    while (true) {
        for (int i = 0; i < 3; i++) {
            for (int j = i; j < LED_COUNT; j += 3) {
                // Build RGB pixels
                hue = j * 360 / LED_COUNT + start_rgb;
                ColorUtils::hsv2rgb(hue, 100, 10, &red, &green, &blue);
                this->led_strip_pixels[j * 3 + 0] = green;
                this->led_strip_pixels[j * 3 + 1] = blue;
                this->led_strip_pixels[j * 3 + 2] = red;
            }
            // Flush RGB values to LEDs
            ESP_ERROR_CHECK(rmt_transmit(led_chan,
                                         led_encoder,
                                         led_strip_pixels,
                                         LED_COUNT*3,
                                         &this->tx_config));
            ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));

            vTaskDelay(pdMS_TO_TICKS(delayMS));
            memset(led_strip_pixels, 0, LED_COUNT * 3);
            ESP_ERROR_CHECK(
                    rmt_transmit(led_chan, led_encoder, led_strip_pixels, LED_COUNT*3, &tx_config));
            ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));

            vTaskDelay(pdMS_TO_TICKS(delayMS));
        }

        start_rgb += 60;
    }

}
