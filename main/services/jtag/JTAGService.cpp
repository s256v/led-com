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

void JTAGService::printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buff[128];
    vsnprintf(buff, 128, format, args);
    usb_serial_jtag_write_bytes(buff, strlen(buff), 20 / portTICK_PERIOD_MS);
    va_end(args);
}

void JTAGService::print(const char *str) {
    this->printf("%s", str);
}

void JTAGService::println(const char *str) {
    this->printf("%s\r\n", str);
}

void JTAGService::printBytes(char *buff, size_t length) {
    this->print("[");
    for (int i = 0; i < length; i++) {
        this->printf("%.2x", buff[i]);
        if (i == length - 1) {
            break;
        }
        this->print(" ");
    }
    this->println("]");
}

void JTAGService::waitFor(const char *str) {
    int index = 0;
    size_t strLength = strlen(str);
    while (true) {
        size_t maxBytesToRead = strLength - index;
        if (maxBytesToRead > JTAG_READ_BUFFER_SIZE) {
            maxBytesToRead = JTAG_READ_BUFFER_SIZE;
        }

        int readBytes = usb_serial_jtag_read_bytes(this->readBuffer, maxBytesToRead,
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

void JTAGService::readBytes(char *buff, size_t length) {
    int index = 0;
    while (true) {
        int readBytes = usb_serial_jtag_read_bytes(this->readBuffer, (JTAG_READ_BUFFER_SIZE),
                                                   20 / portTICK_PERIOD_MS);
        if (!readBytes) {
            continue;
        }

        size_t bytesLeft = length - index;

        if (bytesLeft <= readBytes) {
            memcpy(buff + index, this->readBuffer, bytesLeft);
            return;
        } else {
            memcpy(buff + index, this->readBuffer, readBytes);
            index += readBytes;
            if (index == length) {
                return;
            }
        }
    }
}

