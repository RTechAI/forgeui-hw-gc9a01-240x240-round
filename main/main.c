/*
 * ForgeUI Hardware Lab display-only bring-up entry point.
 * Upstream project attribution is retained in the original source files.
 */
#include "display/gc9a01.h"
#include "display/display.h"
#include "esp_log.h"

void app_main(void)
{
    ESP_LOGI("forgeui", "ForgeUI GC9A01 baseline; Flash: 16 MiB; PSRAM: 8 MiB");
    gc9a01_displayInit();
    displayConfig();
}
