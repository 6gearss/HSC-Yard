# Next Steps: Using HSC-ESP32-Base

## ✅ Current Status

The HSC-ESP32-Base library is **ready to use**! You have:
- ✅ Reusable PlatformIO library structure
- ✅ Embedded web assets (auto-update on recompile)
- ✅ Board type identification system
- ✅ Complete documentation (README, MAINTENANCE)
- ✅ Example application in `src/main.cpp`

## 📦 Preparing for GitHub

### 1. Review and Stage Changes

```bash
cd /Users/dlambert/Projects/antigravity-esp32/HSC-ESP32

# Review what's changed
git status

# Add all new files
git add .gitignore README.md LICENSE MAINTENANCE.md
git add lib/ data/device.html

# Add modified files
git add src/main.cpp src/config.h

# Remove deleted files
git rm data/index.html data/style.css
git rm src/ConfigManager.cpp src/ConfigManager.h
```

### 2. Commit Changes

```bash
git commit -m "Refactor to reusable library structure

- Created HSC_Base library with embedded web assets
- Added board type identification (BOARD_TYPE_DESC, BOARD_TYPE_SHORT)
- Moved ConfigManager to library
- Simplified main.cpp to use library
- Added device.html template for custom pages
- Updated documentation (README, MAINTENANCE)
- Version 0.1.0"
```

### 3. Tag the Release

```bash
git tag -a v0.1.0 -m "Initial release of HSC-ESP32-Base library"
```

### 4. Create GitHub Repository

1. Go to https://github.com/new
2. Repository name: `HSC-ESP32-Base`
3. Description: "Reusable PlatformIO library for ESP32 devices on the HSC train layout"
4. Public or Private (your choice)
5. **Don't** initialize with README (we already have one)
6. Click "Create repository"

### 5. Push to GitHub

```bash
# Add the remote (replace YOUR_USERNAME with your GitHub username)
git remote add origin https://github.com/YOUR_USERNAME/HSC-ESP32-Base.git

# Push code and tags
git push -u origin main
git push --tags
```

## 🚀 Creating Your First Child Project

### Option 1: Using Git Submodule (Recommended)

```bash
# Create new project directory
mkdir ~/Projects/HSC-YardDetector
cd ~/Projects/HSC-YardDetector

# Initialize PlatformIO project
pio project init --board nodemcu-32s

# Add HSC_Base as a submodule
git init
git submodule add https://github.com/YOUR_USERNAME/HSC-ESP32-Base.git lib/HSC_Base

# Copy example files
cp lib/HSC_Base/src/main.cpp src/
cp lib/HSC_Base/src/config.h src/

# Customize src/config.h for your device
# Edit BOARD_TYPE_DESC and BOARD_TYPE_SHORT
```

### Option 2: Copy the Library

```bash
# Create new project
mkdir ~/Projects/HSC-SignalController
cd ~/Projects/HSC-SignalController
pio project init --board nodemcu-32s

# Copy the library
cp -r /Users/dlambert/Projects/antigravity-esp32/HSC-ESP32/lib/HSC_Base lib/

# Copy example files
cp /Users/dlambert/Projects/antigravity-esp32/HSC-ESP32/src/main.cpp src/
cp /Users/dlambert/Projects/antigravity-esp32/HSC-ESP32/src/config.h src/
```

### Customize Your Device

Edit `src/config.h`:

```cpp
#ifndef CONFIG_H
#define CONFIG_H

// Define your specific device type
#undef BOARD_TYPE_DESC
#undef BOARD_TYPE_SHORT
static const char BOARD_TYPE_DESC[] = "Yard Track Detector";
static const char BOARD_TYPE_SHORT[] = "YARD";

// Add device-specific configuration here
// Example: Pin definitions, constants, etc.

#endif
```

Edit `src/main.cpp` to add your device logic:

```cpp
#include "config.h"
#include <HSC_Base.h>
#include <SPIFFS.h>

HSC_Base hscBase;

// Your device-specific variables
const int TRACK_PIN = 5;
bool trackOccupied = false;

void setup() {
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mount failed");
    return;
  }
  
  hscBase.begin();
  
  // Your device initialization
  pinMode(TRACK_PIN, INPUT_PULLUP);
  
  Serial.println("Yard Detector initialized!");
}

void loop() {
  hscBase.loop();
  
  // Your device logic
  bool currentState = digitalRead(TRACK_PIN) == LOW;
  if (currentState != trackOccupied) {
    trackOccupied = currentState;
    
    String topic = "hsc/yard/track/1";
    String payload = trackOccupied ? "OCCUPIED" : "FREE";
    hscBase.getMqttClient().publish(topic.c_str(), payload.c_str(), true);
  }
}
```

### Build and Upload

```bash
pio run --target upload
pio run --target uploadfs  # If using custom files in data/
```

## 🔄 Updating Child Projects

When you update the base library:

### For Git Submodule Projects:
```bash
cd lib/HSC_Base
git pull origin main
cd ../..
pio run --target upload
```

### For Copied Library Projects:
```bash
# Re-copy the library
rm -rf lib/HSC_Base
cp -r /path/to/HSC-ESP32-Base/lib/HSC_Base lib/
pio run --target upload
```

## 📝 Example Device Types

Here are some example board types you might create:

| Device | BOARD_TYPE_DESC | BOARD_TYPE_SHORT | MQTT Example |
|--------|----------------|------------------|--------------|
| Yard Detector | "Yard Track Detector" | "YARD" | `YARD-1` |
| Signal Controller | "Signal Controller" | "SIGNAL" | `SIGNAL-2` |
| Turnout Controller | "Turnout Controller" | "TURNOUT" | `TURNOUT-3` |
| Block Detector | "Block Detector" | "BLOCK" | `BLOCK-4` |
| Sensor Node | "Sensor Node" | "SENSOR" | `SENSOR-5` |

## 🎯 Best Practices

1. **Version Control**: Always use git for your child projects
2. **Board IDs**: Use unique IDs for each physical device (1, 2, 3, etc.)
3. **Testing**: Test on hardware before deploying to layout
4. **Documentation**: Document device-specific features in your project's README
5. **Updates**: Regularly pull library updates to get bug fixes and features

## 📚 Additional Resources

- **README.md**: Quick start and API reference
- **MAINTENANCE.md**: Detailed maintenance and architecture guide
- **lib/HSC_Base/src/HSC_Base.h**: Library API documentation
- **data/device.html**: Template for custom web pages

## 🐛 Troubleshooting

If you encounter issues:
1. Check the serial monitor for error messages
2. Verify WiFi and MQTT settings in web interface
3. Ensure Board ID is set (not 0)
4. Try the "Locate" feature to verify the device is responsive
5. Check MAINTENANCE.md for common issues

---

**Ready to create your first device!** 🚂
