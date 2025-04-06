//
// Created by sergey on 4/6/25.
//

#include <esp_log.h>
#include <cstring>
#include "JTAGService.h"
#include "driver/usb_serial_jtag.h"

static const char *CLASS_TAG = "JTAGService";

void JTAGService::init() {
// Configure USB SERIAL JTAG
    usb_serial_jtag_driver_config_t usb_serial_jtag_config = {
            .tx_buffer_size = JTAG_READ_BUFFER_SIZE,
            .rx_buffer_size = JTAG_READ_BUFFER_SIZE,
    };
    ESP_ERROR_CHECK(usb_serial_jtag_driver_install(&usb_serial_jtag_config));
    ESP_LOGI(CLASS_TAG, "init done");
}

void JTAGService::println(const char *str) {
    char buff[128];
    sprintf(buff, "%s\n", str);
    usb_serial_jtag_write_bytes(buff, strlen(buff), 20 / portTICK_PERIOD_MS);
}

void JTAGService::waitFor(const char *str) {
    int index = 0;
    size_t strLength = strlen(str);
    while (true) {
        int readBytes = usb_serial_jtag_read_bytes(this->readBuffer, (JTAG_READ_BUFFER_SIZE - 1),
                                                   20 / portTICK_PERIOD_MS);
        if (!readBytes) {
            continue;
        }

        for (int i = 0; i < readBytes; i++) {
            if (this->readBuffer[i] == str[index]) {
                index++;
                if (index == strLength) {
                    return; // found
                }
            } else {
                index = 0;
            }
        }
    }

}
