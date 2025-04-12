#include <driver/uart.h>
#include <esp_vfs_dev.h>
#include "services/led/LedService.h"
#include "esp_log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/usb_serial_jtag.h"
#include "services/jtag/JTAGService.h"

#define MAX_ALLOWED_LED_COUNT 256

static const char *TAG = "MAIN";

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "Starting APP");

    JTAGService jtagService;
    jtagService.init();
    jtagService.println("\r\nWelcome to led-com");
    jtagService.println("https://github.com/s256v/led-com");

    uint16_t ledCount;
    while (true) {
        jtagService.waitFor("AT+LN=");
        char buff[2];
        jtagService.readBytes(buff, sizeof(buff));
        ledCount = *reinterpret_cast<uint16_t *>(buff);
        if (ledCount <= MAX_ALLOWED_LED_COUNT) {
            jtagService.printf("AT+LN:OK %d\r\n", ledCount);
            break;
        }
        jtagService.printf("AT+LN:ERR %d > %d\r\n", ledCount, MAX_ALLOWED_LED_COUNT);
    }

    LedService ledService;
    ledService.init(ledCount);
    //ledService.splash(); //todo

    while (true) {
        jtagService.waitFor("AT+RGB=");
        jtagService.readBytes(ledService.getRgbBuffer(), ledService.getRgbBufferSize());
        ledService.refresh();
        jtagService.println("OK");
    }
}