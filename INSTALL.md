# Install on ESP32-S3-Zero (standard Waveshare 4 MB model)

Settings for the standard Waveshare ESP32-S3-Zero (4 MB flash, 2 MB PSRAM). The generic ESP32S3 profile below was used for the successful core 3.3.11 build:

| Arduino setting | Value |
|---|---|
| ESP32 board package | esp32 by Espressif Systems, version 3.3.11 (compile-tested) |
| Board | ESP32S3 Dev Module |
| Flash size | 4MB (32Mb) |
| Flash mode | DIO 80MHz |
| USB Mode | Hardware CDC and JTAG |
| USB CDC On Boot | Enabled |
| Upload Mode | UART0 / Hardware CDC |
| PSRAM | Disabled for this game |
| Partition Scheme | Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS) |

These settings assume the standard ESP32-S3FH4R2 version, with 4 MB flash and 2 MB PSRAM. If your board is a different vendor/variant, confirm its memory specification first. This game does not require PSRAM. The USB-C connector on this board connects directly to native USB; the prior generic UART/COM-connector and 16 MB settings do not apply.

Sources:
- https://docs.waveshare.com/ESP32-S3-Zero
- https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/cdc_dfu_flash.html
- https://github.com/espressif/arduino-esp32/blob/3.3.11/boards.txt

This package contains the latest locally edited sketch: guided training, normalized movement, feedback, and the three-hit shield. Haptics have not been implemented. It is the game firmware; the native/browser emulator stays on your Mac.

1. Install Arduino IDE 2.x from https://www.arduino.cc/en/software/ .
2. In Arduino IDE Settings/Preferences, add this Additional Boards Manager URL if needed:
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
3. In Boards Manager install esp32 by Espressif Systems, version 3.3.11. This version compiled with FastLED 3.10.8; actual audio/LED operation still needs a hardware test. The original author used core 2.0.17.
4. In Library Manager install FastLED version 3.10.8. WiFi, WebServer, Preferences, Update, and driver/i2s.h come from the ESP32 board package.
5. Copy the repository root 1D_SOUND.ino into a folder named 1D_SOUND, then open that copy in Arduino IDE. The folder and sketch names must match.
6. Apply the ESP32-S3-Zero settings in the table above. The standard board has 4 MB flash; use its native USB-C connector and enable USB CDC.
7. Turn off/disconnect the external LED/system supply before connecting the board to the Mac for flashing. Use a USB data cable in the board’s USB-C connector. Select the USB device in Tools > Port (typically /dev/cu.usbmodem... on macOS).
8. Click Verify first, then Upload. If it will not connect, hold BOOT, tap RESET/EN, and release BOOT when upload connects; try holding BOOT during Connecting.
9. After upload, disconnect the computer USB before restoring external system power, following the original project's wiring/power arrangement.
10. Hold Menu for over 3 seconds, then release to enter settings. Join ESP-RGB-INVADERS (password 12345678) and open http://192.168.4.1 . Set your actual LED count, brightness, and sound, then Save. A fresh run starts with three shield points. Hold Menu for about 2 seconds and release for training.

## Wiring (GPIO numbers, not physical header pin numbers)

- LED data: GPIO7
- Red button: GPIO16 to GND
- Green button: GPIO8 to GND
- Blue button: GPIO15 to GND
- White/Menu button: GPIO9 to GND
- MAX98357A BCLK: GPIO4
- MAX98357A LRC/WS: GPIO5
- MAX98357A DIN: GPIO6
- The LED strip and amplifier use external power and share ground with the ESP32. Do not power the strip through the ESP32 board.

## Verification status

Native firmware/emulator checks passed with Green on GPIO8 and White/Menu on GPIO9. The updated full sketch compiled successfully for ESP32-S3 with ESP32 core 3.3.11 and FastLED 3.10.8. No physical ESP32-S3-Zero or audio hardware test has been performed. Do not use the upstream repository's older .bin files: they do not include these local changes.

The original author's non-commercial LICENSE is included. The underlying sketch retains its original settings HTML; the creator/version subtitle and donation footer were hidden by the emulator's local web presentation layer and are not removed from this source package.
