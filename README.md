# ForgeUI GC9A01 240x240 Round Display — ESP32-S3

ForgeUI GC9A01 240x240 Round Display is an official ForgeUI Hardware Lab project developed by [RTechAI](https://github.com/RTechAI). It provides a physically tested ESP32-S3 reference platform for the 1.28-inch GC9A01 round SPI TFT display and a foundation for future ForgeUI Micro Projects and small-screen experiments.

## ForgeUI Ecosystem

ForgeUI is developed by [RTechAI](https://github.com/RTechAI). The [ForgeUI website](https://forgeui.co.nz/) introduces the platform, while [ForgeUI Studio](https://github.com/RTechAI/esp32p4-ui-studio) is the public visual embedded UI/HMI development environment for supported ESP32 hardware.

[ForgeUI Hosted Studio](https://studio.forgeui.co.nz/) is the browser-based ForgeUI Studio application for visual design, native LVGL C generation, browser preview, and supported ESP-IDF build-and-flash workflows. The RTechAI GitHub organisation hosts ForgeUI public repositories, hardware references, framework baselines, examples, and related development work.

This repository is an official RTechAI ForgeUI Hardware Lab project: a physically tested ESP32-S3 and GC9A01 round-display baseline. It does not claim current ForgeUI Studio target integration.

## Physical Validation PASS

This project has been physically tested on:

- ESP32-S3 DevKitC-1 N16R8
- 16 MiB flash
- 8 MiB PSRAM
- 1.28-inch GC9A01 240x240 round TFT

Validated:

- Firmware build and flash
- GC9A01 initialization and SPI rendering
- LVGL startup
- ForgeUI ALIVE showcase
- GPIO10-GPIO14 flat-ribbon wiring

## ForgeUI Micro Projects Platform

This hardware baseline is a reusable foundation for clocks, gauges, instrumentation, animations, Micro Games, and embedded learning projects. Future projects can build from this known-good hardware configuration.

## Hardware

- ESP32-S3 DevKitC-1, N16R8-class: 16 MiB flash and 8 MiB PSRAM
- GC9A01 1.28-inch round SPI TFT, 240x240
- No MISO connection and no separate backlight pin

## Display Wiring

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

GPIO10-GPIO14 are the physically proven flat-ribbon display connection. Do not change them.

## Software Stack

- ESP-IDF 5.5.4
- LVGL 8.3.11
- `espressif/esp_lcd_gc9a01` 1.2.0
- SPI2, mode 0, 20 MHz, RGB565 with `CONFIG_LV_COLOR_16_SWAP=y`
- 16 MiB DIO flash at 80 MHz
- 8 MiB auto-detected octal PSRAM at 80 MHz DDR
- `CONFIG_SPIRAM_USE_MEMMAP=y`: PSRAM is initialized and mapped, without malloc-heap integration

The firmware includes the lightweight LVGL-only ForgeUI ALIVE showcase: an animated circular readiness ring and the display/platform identity.

## Build and Flash

Use ESP-IDF 5.5.4 with the board connected:

```powershell
idf.py build
idf.py -p COMx flash
idf.py -p COMx monitor
```

Replace `COMx` with the detected port. Use `Ctrl-]` to exit the monitor.

## Physical Evidence

| Image | Evidence |
| --- | --- |
| [gc9a01-round-alive-boot.png](docs/images/gc9a01-round-alive-boot.png) | ForgeUI Hardware Lab startup sequence on the ESP32-S3 N16R8. |
| [gc9a01-round-alive-showcase.png](docs/images/gc9a01-round-alive-showcase.png) | Animated ForgeUI GC9A01 LVGL ALIVE showcase. |
| [gc9a01-round-hardware-validation.png](docs/images/gc9a01-round-hardware-validation.png) | Physical GC9A01 240x240 round-display hardware evidence. |

![ForgeUI GC9A01 ALIVE showcase](docs/images/gc9a01-round-alive-showcase.png)

## Related ForgeUI Projects

- [ForgeUI-P4](https://github.com/RTechAI/ForgeUI-P4) — ESP32-P4 LVGL hardware baseline and framework.
- [esp32p4-ui-studio](https://github.com/RTechAI/esp32p4-ui-studio) — public local ForgeUI Studio reference for ESP32-P4 and LVGL 9.
- [ForgeUI-One](https://github.com/RTechAI/ForgeUI-One) — ESP32-P4 LVGL starter baseline and embedded UI framework.

Future ForgeUI Micro Projects may link back to this validated GC9A01 baseline.

## ForgeUI Hardware Lab

ForgeUI Hardware Lab is an RTechAI collection of physically tested ESP32 boards, displays, peripherals, examples, and experimental projects. Each project documents hardware identity, wiring configuration, software baseline, reproducible build process, and physical validation evidence.

This repository is the GC9A01 240x240 round-display reference baseline for future ForgeUI Micro Projects.

## About ForgeUI

ForgeUI is developed by RTechAI. ForgeUI Studio is a visual embedded UI/HMI development environment for supported ESP32 hardware. ForgeUI Hosted Studio is the hosted browser-based ForgeUI Studio application.

- Website: [forgeui.co.nz](https://forgeui.co.nz/)
- Studio: [RTechAI/esp32p4-ui-studio](https://github.com/RTechAI/esp32p4-ui-studio)
- Hosted Studio: [studio.forgeui.co.nz](https://studio.forgeui.co.nz/)
- GitHub: [github.com/RTechAI](https://github.com/RTechAI)

## Attribution

This baseline began from [UsefulElectronics/esp32s3-gc9a01-lvgl](https://github.com/UsefulElectronics/esp32s3-gc9a01-lvgl). Its unrelated application code and generated UI assets were removed from the active baseline. Upstream attribution remains in the retained GC9A01 header. ESP-IDF, LVGL, and the managed GC9A01 component remain subject to their own licences and notices.
