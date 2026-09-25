# ForgeUI GC9A01 240x240 Round Display — ESP32-S3

ForgeUI GC9A01 240x240 Round Display is an official ForgeUI Hardware Lab project developed by RTechAI. It establishes a reproducible ESP32-S3 DevKitC-1 baseline for a 1.28-inch 240x240 round GC9A01 SPI TFT and is intended to become a hardware foundation for ForgeUI Micro Projects and Micro Games experiments.

## Status

**PHYSICAL VALIDATION: PENDING**

The firmware is built for the candidate wiring below. Scott must flash and inspect the physical display before this configuration is described as tested or known-good.

## Project purpose

This is a deliberately minimal round-display bring-up. The firmware starts only the display and LVGL, then renders a static ForgeUI display test. It does not enable Wi-Fi, BLE, sensors, buttons, a joystick, or ForgeUI Studio integration.

## Hardware

- ESP32-S3 DevKitC-1
- 1.28-inch round GC9A01 SPI TFT
- Native resolution: 240x240
- Display PCB markings: `1.28 TFT`, `VER1.0`, `240*240`, `GC9A01`

## Display wiring

| GC9A01 | ESP32-S3 |
| --- | --- |
| VCC | 3.3V |
| GND | GND |
| SCL / SCLK | GPIO4 |
| SDA / MOSI | GPIO5 |
| DC | GPIO6 |
| CS | GPIO7 |
| RST | GPIO15 |

This candidate wiring is pending physical validation. MISO is unused. The module exposes no separate backlight pin.

## Display configuration

The display path uses `espressif/esp_lcd_gc9a01` through ESP-IDF's `esp_lcd` SPI panel API. It uses SPI2 in mode 0 at 20 MHz, 16-bit pixels, BGR byte order, inverted colors, and a horizontally mirrored native 240x240 LVGL display. The ForgeUI test screen has a dark full-screen background, a near-edge circular arc, white/teal/amber/blue labels, and red/green/blue swatches to make coverage, orientation, clipping, colour order, refresh, and corruption easy to inspect.

## Software/build baseline

Repository evidence pins:

- ESP-IDF dependency: 5.5.4 (`dependencies.lock`; manifest permits IDF 4.4 or newer)
- LVGL: 8.3.11 (`dependencies.lock`; manifest requests `~8.3.0`)
- Target: `esp32s3` (`dependencies.lock`)
- GC9A01 driver: `espressif/esp_lcd_gc9a01` 1.2.0

The lockfile also records the Component Manager support dependency `espressif/cmake_utilities` 0.5.3.

## Build and flash

Use an ESP-IDF environment compatible with the locked ESP-IDF 5.3.0 baseline:

```powershell
idf.py set-target esp32s3
idf.py build
idf.py -p COMx flash
idf.py -p COMx monitor
```

Replace `COMx` with the ESP32-S3 serial port. To flash and monitor in one command: `idf.py -p COMx flash monitor`. Exit the monitor with `Ctrl-]`.

## Physical validation record

Physical validation pending.

## ForgeUI Micro Projects direction

This baseline is being evaluated as a reference platform for small ForgeUI projects including clocks, gauges, instrumentation, animations, Micro Games, and Learn / Flash / Hack embedded-code examples. Those features do not currently exist in this repository.

## ForgeUI Hardware Lab

ForgeUI Hardware Lab is an RTechAI/ForgeUI collection of physically tested ESP32 boards, displays, peripherals, examples, and experimental projects. It preserves reproducible hardware baselines through hardware identification, minimal bring-up, physical proof, and known-good configurations, then evaluates demonstrations and candidate targets for future ForgeUI Studio workflows.

This Hardware Lab project does not by itself indicate that this ESP32-S3/GC9A01 target is currently integrated into ForgeUI Studio.

## External dependency and reference attribution

This repository began from [UsefulElectronics/esp32s3-gc9a01-lvgl](https://github.com/UsefulElectronics/esp32s3-gc9a01-lvgl). Existing Useful Electronics source headers, generated SquareLine LVGL files, and their attribution remain in the repository. The active display driver is the separately managed `espressif/esp_lcd_gc9a01` component; its version is pinned in `dependencies.lock` and its licence must be retained according to the Component Registry package.

No top-level upstream licence file is present in the checked-out upstream HEAD. This project therefore does not replace, relabel, or imply ownership of upstream or third-party material. Any future licence addition must preserve applicable upstream and component dependency notices.

## License and repository scope

ForgeUI/RTechAI-authored additions in this baseline are limited to the display-only entry point, candidate wiring configuration, ForgeUI display test, and this documentation. Upstream and third-party material remains subject to its original notices and licences. No new top-level licence is asserted by this change.

## About ForgeUI

ForgeUI is developed by RTechAI.

- ForgeUI: https://forgeui.co.nz/
- ForgeUI Studio: https://studio.forgeui.co.nz/
- RTechAI GitHub: https://github.com/RTechAI

ForgeUI Studio is the visual embedded UI/HMI environment for supported ESP32 hardware. ForgeUI Hardware Lab contains physical hardware references and experimental projects.
