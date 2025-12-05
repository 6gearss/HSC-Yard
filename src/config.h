#ifndef CONFIG_H
#define CONFIG_H

// Override the board type from the library defaults
// This identifies what type of device this is
#undef BOARD_TYPE_DESC
#undef BOARD_TYPE_SHORT

static const char BOARD_TYPE_DESC[] =
    "HSC YARD Device"; // Full description for web UI
static const char BOARD_TYPE_SHORT[] =
    "YARD"; // Short name for MQTT (e.g., "BASE-1", "YARD-2", "SIGNAL-3")

// --- Pin Definitions ---
// Matches the plan: 32, 33, 25, 26, 27, 14
static const int PIN_TRACK_1 = 32;
static const int PIN_TRACK_2 = 33;
static const int PIN_TRACK_3 = 25;
static const int PIN_TRACK_4 = 26;
static const int PIN_TRACK_5 = 27;
static const int PIN_TRACK_6 = 14;
static const int PIN_TRACK_7 = 12;
static const int PIN_TRACK_8 = 13;

// Array for easier iteration
static const int TRACK_PINS[] = {PIN_TRACK_1, PIN_TRACK_2, PIN_TRACK_3,
                                 PIN_TRACK_4, PIN_TRACK_5, PIN_TRACK_6,
                                 PIN_TRACK_7, PIN_TRACK_8};
static const int NUM_TRACKS_PER_BOARD = 8;

// Debounce time in milliseconds
const unsigned long DEBOUNCE_DELAY = 50;

#endif
