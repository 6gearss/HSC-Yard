# HSC-Yard (v0.1.1)

ESP32 firmware for monitoring model train yard tracks, powered by the `HSC_Base` library.

## Hardware

**Board:** ESP32 (NodeMCU-32S)  
**Logic:** 3.3V Active-Low (GND = Occupied)

| Function | GPIO | Note |
| :--- | :--- | :--- |
| **Track 1** | 32 | Input Pullup |
| **Track 2** | 33 | Input Pullup |
| **Track 3** | 25 | Input Pullup |
| **Track 4** | 26 | Input Pullup |
| **Track 5** | 27 | Input Pullup |
| **Track 6** | 14 | Input Pullup |
| **Track 7** | 12 | Input Pullup |
| **Track 8** | 13 | Input Pullup |
| **Reset WiFi**| 4 | Hold 3s |
| **Status LED**| 2 | Locate Blink |

## Software Functions

### Core Library (`HSC_Base`)
- **WiFi**: Auto-connects to configured SSID. Fallback AP: `HSC-Setup` (pass: `password`).
- **MQTT**: Auto-reconnects. Configurable broker.
- **Web UI**: Configuration portal at device IP.

### Application Logic
- **Monitoring**: Debounces inputs (50ms) to detect train presence.
- **Reporting**: Publishes state changes to MQTT.
  - Topic: `HSC/yard/track/{TRACK_NUM}/section/{BOARD_ID}`
  - Payload: `OCCUPIED` or `FREE` (Retained)

## Usage

1.  **Flash**: Run `./flash.sh`
2.  **Configure**:
    - Connect to device IP.
    - Set **Board ID** (Section #) and **Location**.
3.  **Monitor**: Open `web/dashboard.html` in a browser for a real-time view.
