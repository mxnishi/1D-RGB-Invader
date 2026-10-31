# RGB Invader enhanced firmware emulator

Source-level emulation of the locally edited mxnishi/1D-RGB-Invader fork, based on upstream commit f418a1919d2f42cf02ab47cb2e5d1d3255b6c362. The original firmware's non-commercial LICENSE is included. `upstream.ino` retains the upstream snapshot; `firmware.ino` is the enhanced sketch used by the emulator.

## Run

From this folder, with Python 3 and Clang installed:

```sh
python3 build.py
python3 server.py
```

Open http://127.0.0.1:8766. Build the native emulator on your computer with `python3 build.py`; generated executables are not committed. No Python packages are required. Set RGB_EMULATOR_PORT to change ports. Stop with Ctrl+C.

A new campaign has a **three-hit shield**. Breaches cost one point; the third ends the run. Shield carries across levels and refills on a new run. Surviving hits knock enemies back, clear boss projectiles, and give one second of recovery. Training and bonus lives are independent.

Press **Start training** for nine guided lessons, or **Run firmware** for the current campaign. A/S/D are RGB; simultaneous keys make mixed colors. Space pauses the clock. A quick M restarts the configured campaign; hold 1–3 virtual seconds and release for training; hold over 3 seconds and release for Wi-Fi settings. Scene shortcuts enter a level or bonus without winning earlier stages. Training is unscored and does not change gameplay statistics.

Training instructions and details of normalized movement and feedback are in ENHANCEMENTS.md. The settings page's Start with training preference survives virtual reboots. Saving the main firmware settings restarts the native process. Emulator settings pages omit the requested creator/version subtitle and donation footer; source attribution and licensing remain in the package.

Preferences and scores persist in preferences.txt, separate from any real board. Use dummy Wi-Fi credentials; preferences are stored locally as plaintext. OTA is unavailable. One virtual board is shared across tabs; run only one emulator play tab at a time. Refreshing the play page pauses the clock but preserves board state.

## Fidelity

The actual C++ sketch supplies the campaign, bosses, rage/phase logic, bonuses, collisions, scoring, the three-hit shield, training, normalized movement, feedback, and generated settings pages. build.py adds Arduino forward declarations to sketch.generated.cpp; host.cpp bridges inputs and snapshots; shims/ supplies peripheral adapters.

This is not ESP32 instruction-set or binary emulation. CPU/peripheral timing, radio, RTOS scheduling, electrical behavior, power limiting, I²S DMA, and OTA are not modeled. HSV colors and audio-event scheduling are approximated. Firmware melodies play through Web Audio; the FreeRTOS audio task is compiled but not scheduled. Random sequences differ from ESP32. Preferences blobs use the host format and cannot be imported into ESP32 NVS. The browser's flash smoothing affects transitions only, keeping shot colors immediate. Settings handlers receive bounded inputs from the local bridge.

Physical board validation remains outstanding; see ENHANCEMENTS.md for the required Arduino toolchain.

## Regression checks

```sh
python3 build.py
clang++ -std=c++17 -O1 -Wno-c++11-narrowing -Ishims tests.cpp -o /tmp/rgb-firmware-tests
/tmp/rgb-firmware-tests
clang++ -std=c++17 -O1 -Wno-c++11-narrowing -Ishims enhancements-tests.cpp -o /tmp/rgb-enhancement-tests
/tmp/rgb-enhancement-tests
clang++ -std=c++17 -O1 -Wno-c++11-narrowing -Ishims shield-tests.cpp -o /tmp/rgb-shield-tests
/tmp/rgb-shield-tests
node test-renderer.cjs
```

Tests exercise real sketch functions. Fixtures inject shots and clear projectiles where needed to isolate boss transitions. Browser verification covers training entry and first-lesson progression; a native process integration check covers saving the training-on-start preference and restoring it on reboot. Tests establish source behavior, not physical hardware equivalence.

Shield regressions cover all three bosses, body/projectile breaches, recovery, duplicate game-end protection, level/bonus carry-over, training isolation, new-run refill, and physical base charge indication.
