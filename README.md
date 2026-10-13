# ESP32 LVGL Dashboard

This is an ESP-IDF + LVGL scaffold for a 300 x 400 display shown in landscape as 400 x 300.

Design constraints:

- Screen: 300 x 400 panel, displayed as 400 x 300 landscape
- Palette: black, red, and grayscale only
- Reflective look: gray paper background, low-contrast texture, black/red ink
- Pages: home, notes, calendar, weather/environment, widgets
- Hardware features are isolated behind `main/ports`
- Sync/weather/sensor data are isolated behind `main/services`

Current state:

- The UI is implemented with mock data so the page structure can be verified early.
- Display output uses a null LVGL display port until a real panel driver is selected.
- IMU gestures are modeled in `services/imu_service.*` and `ports/imu_port.*`.

Suggested next hardware decisions:

- Display controller and bus: SPI/QSPI/RGB/I80
- Touch or button input wiring
- IMU model and I2C/SPI pins
- Sensor list for environment data
- Upstream sync protocol: UART, BLE, Wi-Fi HTTP, MQTT, or USB CDC

Build:

```powershell
idf.py set-target esp32
idf.py build
```

PC simulator:

```powershell
cd F:\MCU\lvgl\ESP32
cmake -S simulator -B simulator\build -G "MinGW Makefiles" -DSDL2_DIR=C:/SDL2-2.32.4/x86_64-w64-mingw32/lib/cmake/SDL2
cmake --build simulator\build
.\simulator\bin\esp32_ui_sim.exe
```

Chinese font subset:

```powershell
cd F:\MCU\lvgl\ESP32
python .\tools\gen_app_cjk_font.py
cmake --build simulator\build
```

The generated font files are stored in `main/assets/fonts`. If new Chinese UI text is added, update `CHARS` in `tools/gen_app_cjk_font.py` and regenerate the font.

Main folders:

- `main/ui`: LVGL screens and page layout
- `main/core`: app state and theme constants
- `main/services`: sync, weather, calendar, sensor, and IMU logic
- `main/ports`: hardware-facing adapters
