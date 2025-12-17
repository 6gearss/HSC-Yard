# HSC_Base Library

Base library for HSC ESP32 devices. Handles common functionality:
- WiFi Connection (Station + Fallback AP)
- MQTT Connection & Status Reporting
- Web Server (Configuration, Status, Firmware Updates)
- OTA Firmware Updates via Web UI

## Version 0.2.0
- Added Over-The-Air (OTA) firmware update support.
- Added `/firmware` page for managing updates.
- Added `/api/firmware/check` and `/api/update` endpoints.
- Added `setUpdateUrl()` and `setBoardInfo()` version parameter.

## Usage

Include in your `platformio.ini`:
```ini
lib_deps =
    HSC_Base
    knolleary/PubSubClient
    esphome/ESPAsyncWebServer-esphome
    esphome/AsyncTCP-esphome
    bblanchon/ArduinoJson
```

Initialize in `main.cpp`:
```cpp
#include "HSC_Base.h"
#include "config.h"

HSC_Base hscBase;

void setup() {
    hscBase.setBoardInfo("My Device", "DEVICE-1", "1.0.0");
    hscBase.setUpdateUrl("http://my-server/firmware_%BOARD_TYPE%.bin");
    hscBase.begin();
}

void loop() {
    hscBase.loop();
    // Your code...
}
```
