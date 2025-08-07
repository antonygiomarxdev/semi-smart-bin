#pragma once

// Serial
static const int SERIAL_BAUD = 9600;

// Modos de ejecución
static constexpr bool PRODUCTION_MODE = false;
static constexpr bool DEBUG = !PRODUCTION_MODE;
static constexpr bool DEEP_DEBUG = false && !PRODUCTION_MODE;

// Intervalos y tiempos (ms)
static const unsigned long UPDATE_INTERVAL_MS = 30;        // 33 Hz
static const unsigned long ANALYSIS_TIME_MS = 500;         // 0.5 s
static const unsigned long RESULT_TIME_MS = 7000;          // 7 s (espera tras cada clasificación)
static const unsigned long INACTIVITY_TIMEOUT_MS = 15000;  // 15 s
static const unsigned long INACTIVE_DISPLAY_MS = 1500;     // 1.5 s

// Umbrales
static const int DIST_THRESHOLD_CM = 10;
static const float COLOR_DIST_THRESHOLD = 0.35f;

// Debounce
static constexpr int PRESENCE_COUNT_THRESHOLD = 2;

// Cambio de color para re-análisis
static constexpr float COLOR_CHANGE_THRESHOLD = 0.10f;
static constexpr float COLOR_CHANGE_THRESHOLD2 = COLOR_CHANGE_THRESHOLD * COLOR_CHANGE_THRESHOLD;

// Sensor de color
static const int COLOR_SAMPLE_COUNT = 5;
