# HSC-ESP32-Base Library Maintenance Guide

## Overview
The HSC-ESP32-Base project is structured as a **PlatformIO library** that provides core functionality for ESP32 projects. This allows child projects to automatically receive updates by simply recompiling.

## Architecture

### Library Structure
```
lib/HSC_Base/
├── src/
│   ├── HSC_Base.h          # Main library header
│   ├── HSC_Base.cpp        # Main library implementation (with embedded web assets)
│   ├── ConfigManager.h     # Configuration management
│   ├── ConfigManager.cpp   # Configuration implementation
│   └── config.h            # Default configuration constants
```

### Embedded Web Assets
The library embeds `index.html` and `style.css` directly in `HSC_Base.cpp` using raw string literals (`R"rawliteral(...)`). This ensures that:
- **Updates propagate automatically**: When you update the library, child projects get the new web interface by recompiling.
- **No file copying needed**: Child projects don't need to maintain their own copies of these files.

### Child Project Structure
```
src/
└── main.cpp                # Minimal application code using HSC_Base

data/
└── device.html             # Optional device-specific pages

lib/                        # (empty - library is in parent repo)
```

## Maintenance Workflows

### Updating the Base Library (This Repo)

#### 1. Update Web Interface
To update `index.html` or `style.css`:

1. Edit the embedded strings in `lib/HSC_Base/src/HSC_Base.cpp`
2. Find the `index_html` or `style_css` raw string literal
3. Make your changes
4. Compile and test: `pio run`
5. Commit and push changes

**Example:**
```cpp
static const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
...
)rawliteral";
```

#### 2. Update Core Functionality
To update WiFi, MQTT, or Web Server logic:

1. Edit `lib/HSC_Base/src/HSC_Base.cpp` or `HSC_Base.h`
2. Compile and test: `pio run`
3. Commit and push changes

#### 3. Update Default Configuration
To update default WiFi/MQTT settings:

1. Edit `lib/HSC_Base/src/config.h`
2. Update constants like `WIFI_SSID`, `MQTT_SERVER`, etc.
3. Compile and test: `pio run`
4. Commit and push changes

### Using the Library in Child Projects

#### Option 1: Git Submodule (Recommended)
```bash
# In your new project
git submodule add <this-repo-url> lib/HSC_Base

# Update to latest
git submodule update --remote lib/HSC_Base
pio run
```

#### Option 2: Copy Library
```bash
# Copy the library folder
cp -r /path/to/HSC-ESP32/lib/HSC_Base /path/to/child-project/lib/

# To update: repeat the copy and recompile
pio run
```

#### Option 3: PlatformIO Library (Future)
Publish to PlatformIO Registry and use:
```ini
[env:nodemcu-32s]
lib_deps = 
    HSC_Base
```

### Creating a Child Project

1. **Create new PlatformIO project**:
   ```bash
   pio project init --board nodemcu-32s
   ```

2. **Add HSC_Base library** (using one of the options above)

3. **Create minimal `main.cpp`**:
   ```cpp
   #include "config.h"
   #include <HSC_Base.h>
   #include <SPIFFS.h>

   HSC_Base hscBase;

   void setup() {
     if (!SPIFFS.begin(true)) {
       Serial.println("SPIFFS mount failed");
       return;
     }
     
     hscBase.begin();
     
     // Add device-specific initialization
   }

   void loop() {
     hscBase.loop();
     
     // Add device-specific loop code
   }
   ```

4. **Optional: Add device-specific page**:
   Create `data/device.html` and register it:
   ```cpp
   hscBase.registerPage("/device", [](AsyncWebServerRequest *request) {
     request->send(SPIFFS, "/device.html", String(), false);
   });
   ```

5. **Compile and upload**:
   ```bash
   pio run --target upload
   pio run --target uploadfs  # If using device.html
   ```

## Git Repository Structure

### This Repository (HSC-ESP32-Base)
This repo serves dual purposes:
1. **Library source**: `lib/HSC_Base/` contains the reusable library
2. **Example/template**: `src/main.cpp` shows how to use the library

### Recommended Workflow
1. Keep this repo as the **source of truth** for the library
2. Use Git tags for versions: `git tag v0.1.0`
3. Child projects reference specific tags or track `main`

## Updating Child Projects

When the base library is updated:

1. **Pull latest changes** (if using submodule):
   ```bash
   cd lib/HSC_Base
   git pull origin main
   cd ../..
   ```

2. **Recompile**:
   ```bash
   pio run
   ```

3. **Upload**:
   ```bash
   pio run --target upload
   ```

That's it! The new web interface and functionality are now active.

## Key Benefits

✅ **Automatic updates**: Recompiling gets latest features  
✅ **No file management**: Web assets are embedded  
✅ **Modular**: Easy to add device-specific features  
✅ **Consistent**: All devices share the same base UI/functionality  
✅ **Version control**: Use Git tags to manage library versions  

## Troubleshooting

### "config.h not found" error
- Ensure `lib/HSC_Base/src/config.h` exists
- The library includes its own copy of config.h

### Web page not updating
- Verify you edited the embedded string in `HSC_Base.cpp`, not a separate HTML file
- Recompile completely: `pio run --target clean && pio run`

### SPIFFS errors in child project
- Ensure `SPIFFS.begin(true)` is called in `setup()`
- Only needed if using `device.html` or other custom files
