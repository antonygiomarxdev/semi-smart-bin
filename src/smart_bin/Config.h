#pragma once

// Para el Serial Monitor
static const int SERIAL_BAUD = 9600;
static const int BT_BAUD = 9600;

// Modos de ejecución
static constexpr bool PRODUCTION_MODE = true;
static constexpr bool DEBUG = !PRODUCTION_MODE;
static constexpr bool DEEP_DEBUG = true && !PRODUCTION_MODE;

// Tiempos (ms)
static const unsigned long ANALYSIS_TIME_MS = 1000;
static const unsigned long RESULT_TIME_MS = 7000;
static const unsigned long INACTIVITY_TIMEOUT_MS = 15000;
static const unsigned long INACTIVE_DISPLAY_MS = 1500;


// Umbrales
static const int DIST_THRESHOLD_CM = 20;
static const float COLOR_DIST_THRESHOLD = 0.35f;
static const int COLOR_SAMPLE_COUNT = 10;


static const unsigned long UPDATE_INTERVAL_MS = 50;
