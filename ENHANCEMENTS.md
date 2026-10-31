# Shield, training, consistent movement, and feedback

Local changes based on upstream commit f418a1919d2f42cf02ab47cb2e5d1d3255b6c362. Original author and license remain in the repository.

## Three-hit shield

A new main-game run starts with three shield points. A normal enemy, boss body, or boss projectile reaching the homebase costs one point. The third hit starts the existing base-destruction/game-over sequence and records the run once. Wrong-color shots retain their existing penalties; they do not directly consume shield.

A nonfatal hit clears incoming boss projectiles, knocks the enemy front back to at least 25% of the playable track, and provides one second of recovery. Breaches during recovery do not consume more charge, preventing one burst or overlapping contact from using the whole shield. The boss's armor, phase, and the current level remain intact.

Shield carries across level and bonus transitions. A quick menu restart, normal boot, or a fresh emulator level scene refills it. Training does not consume shield; finishing training begins a new campaign. Beat Saber and Simon Says keep their separate lives and do not spend campaign shield.

The default three-pixel homebase shows one illuminated LED per remaining shield point. Other base sizes show a fractional charge bar. A nonfatal hit produces a red-orange base cue, a shield-impact sound, and an emulator text message. The browser HUD displays three shield icons and a numeric count, plus recovery/bonus/training context. This update does not drive physical haptic motors.

## Guided training

Enter using START TRAINING in the settings page, or hold the menu button for about two seconds and release. A quick menu tap starts a normal campaign; a hold longer than three seconds enters Wi-Fi settings. The optional Start with training checkbox persists across reboots. Existing installations default to normal campaign startup.

The nine lessons are:

1. Red (red button / A in the emulator).
2. Green (green / S).
3. Blue (blue / D).
4. Deliberately fire blue or green at red to see a wrong-color shot add an enemy.
5. Yellow: red + green.
6. Magenta: red + blue.
7. Cyan: green + blue.
8. White: all three buttons.
9. Clear three slowly moving targets: red, green, then blue.

Training has no scoring or statistics writes. Incorrect attempts can be retried indefinitely; a moving target reaching the base resets safely. Mixed-color practice allows 120 ms to assemble the chord; campaign timing remains 60 ms. Release between attempts. Completed lessons light nine dim progress markers at the far end of the strip. At completion, release and press any color to begin the configured campaign.

The emulator supplies text instructions. On the physical strip, use the numbered lesson guide above: each target shows the required color, and lesson four is an intentional wrong-color demonstration.

## Movement

A 240-LED reference strip establishes the existing nominal speed scale. Movement is multiplied by playable-distance ratio and elapsed seconds per loop; the time step is capped at 64 ms after stalls. Enemies, player shots, boss motion, boss projectiles, Simon Says movement, and Beat Saber movement all use that scale. Markers use playable-distance percentages. The Overlord firing limit also scales rather than changing abruptly at 180 LEDs.

The base's pixel count remains configurable. Wave/enemy/armor counts, health, fire cadence, input cooldown, combo timing, and score multipliers retain their existing rules. Normalization equalizes travel time, not every aspect of difficulty: fixed-size groups occupy more of a short strip and rendering is coarser on fewer LEDs.

The non-Masterblaster score target uses the reference travel distance so identical completion times have consistent time bonuses. Swept collision checks prevent faster long-strip shots skipping bonus targets or front boss segments.

Regression measurements with no input on Standard level one: 100 LEDs 47,200 ms, 240 LEDs 47,168 ms, 600 LEDs 47,200 ms to base, at both 1 ms and 32 ms scheduler increments. Reference speeds now follow actual time rather than assuming exactly 60 frames per second; this slightly changes reference-board timing relative to the original 16 ms loop.

## Feedback

- Green base pulse: successful hit or projectile interception.
- Amber base pulse: wrong color.
- Violet base pulse: boss currently protected.
- Steady orange base: enemy/projectile within the last 15% of the playable track.
- Cyan: training lesson cue.

Hit, mistake, blocked, and danger sounds are distinct. All honor the existing sound/volume controls. The emulator also shows a text feedback banner from the firmware event data. These cues use steady short pulses rather than adding rapid flashes.

## Validation and deployment

The native firmware harness verifies training completion and isolation from scoring/statistics, menu access, normal and boss collisions, phase logic, bonuses, frame/length normalization, feedback, long-strip collision sweeps, and startup preference persistence. The emulator uses this edited sketch; its upstream snapshot is retained separately.

The full ESP32-S3 sketch compiles with ESP32 core 3.3.11 and FastLED 3.10.8 (4 MB flash, DIO, USB CDC enabled, PSRAM disabled, Minimal SPIFFS with OTA). The legacy I2S audio API remains available; a compile does not confirm physical audio quality or concurrent LED/audio reliability. Core 2.0.17 is the original author's baseline. No physical board has been flashed or tested. Do not use the older precompiled upstream binaries for these enhancements.

## ESP32-S3-Zero button wiring

Green uses GPIO8 and White/Menu uses GPIO9, both connected through their buttons to GND. These edge pins replace underside GPIO17/18. Internal pull-ups remain enabled. Red remains GPIO16 and Blue remains GPIO15.
