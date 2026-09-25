/*
 * ForgeUI Hardware Lab display-only bring-up entry point.
 * Upstream project attribution is retained in the original source files.
 */
#include "display/display.h"
#include "display/gc9a01.h"

void app_main(void)
{
    gc9a01_displayInit();
    displayConfig();
}
