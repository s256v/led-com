#include "services/led/LedService.h"
#include "esp_log.h"

static const char *TAG = "MAIN";

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "Starting APP");
    LedService ledService;
    ledService.init();
    ledService.startLoop();
}
