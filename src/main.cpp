#include "config.h"
#include <HSC_Base.h>
#include <SPIFFS.h>

HSC_Base hscBase;

// State tracking
int lastState[NUM_TRACKS_PER_BOARD] = {LOW};
unsigned long lastDebounceTime[NUM_TRACKS_PER_BOARD] = {0};
int buttonState[NUM_TRACKS_PER_BOARD] = {LOW};

bool wasConnected = false;

void publishTrackState(int trackIndex, int state) {
  if (hscBase.getConfig().board_id == 0)
    return;

  // Transposed Logic from YardDetector:
  // trackIndex corresponds to TRACK index (0-5 -> Track 1-6)
  // BOARD_ID corresponds to SECTION ID
  String topic = "HSC/yard/track/" + String(trackIndex + 1) + "/section/" +
                 String(hscBase.getConfig().board_id);
  String payload = (state == LOW) ? "OCCUPIED" : "FREE";

  Serial.print("Publishing to ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(payload);

  if (hscBase.getMqttClient().connected()) {
    hscBase.getMqttClient().publish(topic.c_str(), payload.c_str(),
                                    true); // Retained
  }
}

void publishAllTracks() {
  Serial.println("Publishing all track states...");
  for (int i = 0; i < NUM_TRACKS_PER_BOARD; i++) {
    publishTrackState(i, buttonState[i]);
  }
}

void setup() {
  // Initialize the HSC_Base library
  hscBase.setBoardInfo(BOARD_TYPE_DESC, BOARD_TYPE_SHORT, FW_VERSION);
  hscBase.setUpdateUrl(UPDATE_URL);
  hscBase.begin();

  // Initialize Pins
  for (int i = 0; i < NUM_TRACKS_PER_BOARD; i++) {
    // OI-IB-8 uses open-collector/active-low output:
    // - When train is present (OCCUPIED): pulls pin to GND (LOW)
    // - When no train (FREE): releases pin (floating)
    // INPUT_PULLUP pulls the pin HIGH when floating, LOW when grounded
    pinMode(TRACK_PINS[i], INPUT_PULLUP);

    // Initialize state
    int reading = digitalRead(TRACK_PINS[i]);
    lastState[i] = reading;
    buttonState[i] = reading;
  }

  // Register device-specific page
  hscBase.registerPage("/device", [](AsyncWebServerRequest *request) {
    request->send(
        SPIFFS, "/device.html", String(), false,
        [](const String &var) { return hscBase.processTemplate(var); });
  });
}

void loop() {
  // Run the HSC_Base loop
  hscBase.loop();

  // Handle MQTT Connection State Changes
  bool isConnected = hscBase.getMqttClient().connected();
  if (isConnected && !wasConnected) {
    // Just connected
    publishAllTracks();
  }
  wasConnected = isConnected;

  // Read Pins
  for (int i = 0; i < NUM_TRACKS_PER_BOARD; i++) {
    int reading = digitalRead(TRACK_PINS[i]);

    if (reading != lastState[i]) {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;
        // State Changed, Publish
        publishTrackState(i, buttonState[i]);
      }
    }

    lastState[i] = reading;
  }
}
