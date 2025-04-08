#include <driver/uart.h>
#include <esp_vfs_dev.h>
#include "services/led/LedService.h"
#include "esp_log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/usb_serial_jtag.h"
#include "services/jtag/JTAGService.h"


static const char *TAG = "MAIN";

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "Starting APP");

    JTAGService jtagService;
    jtagService.init();
    jtagService.println("\r\nWelcome to led-com");
    jtagService.println("https://github.com/s256v/led-com");

    LedService ledService;
    ledService.init();
    ledService.splash();

    while (true) {
        jtagService.waitFor("AT+RGB=");
        jtagService.readBytes(ledService.getRgbBuffer(), ledService.getRgbBufferSize());
        ledService.refresh();
        jtagService.println("OK");
    }
}