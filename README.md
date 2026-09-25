# ForgeUI GC9A01 240×240 Round Display

ForgeUI GC9A01 240×240 Round Display is an official ForgeUI Hardware Lab project developed by [RTechAI](https://github.com/RTechAI). It provides a physically tested ESP32-S3 baseline for the GC9A01 1.28-inch 240×240 round SPI TFT and forms part of the wider ForgeUI ESP32 hardware, Micro Projects and embedded UI ecosystem.

The low-level ESP-LCD display path is physically proven. LVGL rendering remains under investigation after a minimal LVGL proof screen produced a black physical display.

## Status

- **Low-level ESP-LCD display path: physical pass** — the ESP32-S3, GC9A01 initialization, GPIO10–GPIO14 ribbon wiring, SPI communication, and full-screen red, green, blue, white, and black rendering were verified on the target hardware.
- **LVGL display path: physical validation fail / investigation open** — the current minimal LVGL proof, intended to show a red background with centred white `FORGEUI LVGL` text, remained physically black. Its runtime and flush instrumentation, including the `LV_COLOR_16_SWAP` byte-order investigation, are retained for diagnosis.

## Purpose

This repository is a small, cloneable baseline for an ESP32-S3 driving a round GC9A01 through ESP-IDF. It is intentionally limited to the physically proven low-level panel path and an LVGL diagnostic path. Future ForgeUI projects may build on the proven panel/wiring baseline, but LVGL transport must first be resolved and physically validated.

## Hardware

- ESP32-S3 DevKitC-1 N16R8-class board: 16 MiB flash and 8 MiB octal PSRAM
- 1.28-inch round GC9A01 SPI TFT, 240×240
- PCB marking: `1.28 TFT`, `VER1.0`, `240*240`, `GC9A01`

## Locked display wiring

| GC9A01 | ESP32-S3 |
| --- | --- |
| VCC | 3.3V |
| GND | GND |
| SCL / SCLK | GPIO10 |
| SDA / MOSI | GPIO11 |
| DC | GPIO12 |
| CS | GPIO13 |
| RST | GPIO14 |
| MISO | Unused |

The five signal GPIOs are intentionally adjacent for the ForgeUI Micro Projects flat-ribbon connection. This module has no separate backlight pin.

## Software baseline

- ESP-IDF 5.5.4
- LVGL 8.3.11
- `espressif/esp_lcd_gc9a01` 1.2.0
- SPI2, mode 0, 20 MHz, RGB565 (BGR panel order), hardware reset, inverted colour, and horizontal mirror
- `sdkconfig` and `sdkconfig.defaults` declare the proven board’s 16 MiB flash and 8 MiB octal PSRAM; the simple single-app partition layout is retained.

## LVGL diagnostic status

The current firmware intentionally renders one minimal LVGL proof screen: a solid red background and centred white `FORGEUI LVGL` text. The physical display remains black with this firmware. This is not a finished showcase and is not a physically proven LVGL baseline.

LVGL receives a 2 ms tick from `esp_timer`; one FreeRTOS task calls `lv_timer_handler()` every 5 ms. The ESP-IDF panel-transfer callback calls `lv_disp_flush_ready()` when DMA drawing completes. `main/display/display.c` logs driver registration, task progress, flush entry, and completion counts to support the investigation. The direct ESP-LCD transfer path remains the known-good comparison.

## Build, flash, and monitor

Use ESP-IDF 5.5.4 with the target board connected:

```powershell
idf.py build
idf.py -p COMx flash
idf.py -p COMx monitor
```

Replace `COMx` with the detected serial port. Use `Ctrl-]` to leave the monitor.

## Related ForgeUI Projects

- [ForgeUI-P4](https://github.com/RTechAI/ForgeUI-P4) — RTechAI’s ESP32-P4 LVGL hardware baseline and framework.

Future GC9A01 projects may link to the physically proven low-level panel baseline in this repository. No LVGL-ready or Micro Game baseline is claimed until LVGL has been physically validated.

## About ForgeUI

ForgeUI is developed by RTechAI and focuses on visual embedded UI/HMI development alongside physically proven ESP32 workflows. ForgeUI Hardware Lab provides reproducible hardware references, examples, and experimental projects. This repository is one Hardware Lab baseline; it does not by itself mean that the GC9A01 target is integrated into ForgeUI Studio.

- [RTechAI on GitHub](https://github.com/RTechAI)
- [ForgeUI](https://forgeui.co.nz/)
- [ForgeUI Studio](https://studio.forgeui.co.nz/)

## Attribution and scope

This repository began from [UsefulElectronics/esp32s3-gc9a01-lvgl](https://github.com/UsefulElectronics/esp32s3-gc9a01-lvgl). Its upstream application code and generated assets were removed from the active baseline because they were not part of the display-only project. The ForgeUI baseline retains no claim over upstream or third-party software. ESP-IDF, LVGL, and the managed GC9A01 component remain subject to their own licenses and notices.

No top-level upstream license file was present in the checked-out upstream reference. Do not infer or add an upstream license without confirming it separately.
