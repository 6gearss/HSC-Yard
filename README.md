# HSC-Yard

A specialized ESP32 application for monitoring model train yard tracks, built on the `HSC_Base` library.

## Overview

This project monitors 8 track sections for occupancy and reports their status via MQTT. It is designed to work with the HSC layout automation system.

## Library

This project uses the **HSC_Base** library (`lib/HSC_Base`) for core functionality:
- WiFi connection and management (with AP fallback)
- MQTT client handling
- Web configuration interface
- OTA updates
- Configuration persistence

## Functionality

1.  **Track Monitoring**:
    - Monitors 8 GPIO pins for track occupancy.
    - Uses active-low logic (LOW = OCCUPIED, HIGH = FREE).
    - Implements 50ms debouncing for reliable detection.

2.  **MQTT Reporting**:
    - Publishes status changes immediately to: `HSC/yard/track/{TRACK_NUM}/section/{BOARD_ID}`
    - Payload: `OCCUPIED` or `FREE`
    - Retained messages ensure state is known on subscriber connection.
    - Publishes all track states on initial MQTT connection.

3.  **Web Interface**:
    - **Configuration**: Set WiFi, MQTT, Board ID, and Location.
    - **Device Page**: Custom status page at `/device`.
    - **Dashboard**: A visual representation of the yard is available in `web/dashboard.html` (runs locally).

## Hardware Specifications

- **Microcontroller**: ESP32 (NodeMCU-32S)
- **Input Voltage**: 5V (via USB or VIN)
- **Logic Level**: 3.3V

### Pinout

| Track # | GPIO Pin | Description |
| :--- | :--- | :--- |
| Track 1 | 32 | Input (Pullup) |
| Track 2 | 33 | Input (Pullup) |
| Track 3 | 25 | Input (Pullup) |
| Track 4 | 26 | Input (Pullup) |
| Track 5 | 27 | Input (Pullup) |
| Track 6 | 14 | Input (Pullup) |
| Track 7 | 12 | Input (Pullup) |
| Track 8 | 13 | Input (Pullup) |
| AP Button | 4 | Hold 3s to reset WiFi |
| Status LED | 2 | Blinks for "Locate" |

**Note**: Inputs are configured with internal pull-ups. Sensors should pull the pin to GND when occupied.

## Quick Start

1.  **Build & Upload**:
    ```bash
    ./flash.sh
    ```
2.  **Monitor**:
    ```bash
    ./monitor.sh
    ```
3.  **Configure**:
    - Connect to the device IP.
    - Set **Board ID** (this corresponds to the *Section* number).
    - Set **Location** (e.g., "Staging Yard").

## Dashboard

A local HTML dashboard is provided in `web/dashboard.html`. Open this file in a web browser to view the real-time status of the yard tracks. It connects directly to the MQTT broker.
