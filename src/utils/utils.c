/**
 * @file utils.c
 * @brief Implémentation des utilitaires
 */

#include "utils.h"
#include "../config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ============================================================================
// LOGGING
// ============================================================================

void utils_log(const char *level, const char *msg) {
    if (!DEBUG_MODE) return;
    
    Serial.print("[");
    Serial.print(level);
    Serial.print("] ");
    Serial.println(msg);
}

void utils_log_int(const char *level, const char *msg, int value) {
    if (!DEBUG_MODE) return;
    
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s: %d", msg, value);
    utils_log(level, buffer);
}

void utils_log_float(const char *level, const char *msg, float value) {
    if (!DEBUG_MODE) return;
    
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s: %.2f", msg, value);
    utils_log(level, buffer);
}

// ============================================================================
// MATH
// ============================================================================

float utils_map(float x, float in_min, float in_max,
                float out_min, float out_max) {
    if (in_max == in_min) return out_min;
    
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float utils_constrain(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

float utils_distance_haversine(double lat1, double lon1,
                               double lat2, double lon2) {
    const double R = 6371000.0;  // Rayon terrestre en mètres
    
    double phi1 = lat1 * M_PI / 180.0;
    double phi2 = lat2 * M_PI / 180.0;
    double delta_phi = (lat2 - lat1) * M_PI / 180.0;
    double delta_lambda = (lon2 - lon1) * M_PI / 180.0;
    
    double a = sin(delta_phi / 2) * sin(delta_phi / 2) +
               cos(phi1) * cos(phi2) * 
               sin(delta_lambda / 2) * sin(delta_lambda / 2);
    
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    
    return (float)(R * c);
}

// ============================================================================
// STRING
// ============================================================================

void utils_float_to_str(float value, char *str, int decimals) {
    char format[16];
    snprintf(format, sizeof(format), "%%.%df", decimals);
    snprintf(str, 32, format, value);
}

void utils_itoa(int value, char *str, int base) {
    // Utiliser itoa standard si disponible
    #ifdef __AVR__
        itoa(value, str, base);
    #else
        snprintf(str, 32, "%d", value);
    #endif
}

// ============================================================================
// CRC & VALIDATION
// ============================================================================

uint8_t utils_crc8(const uint8_t *data, size_t len) {
    uint8_t crc = 0;
    
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    
    return crc;
}

bool utils_validate_nmea(const char *sentence) {
    if (!sentence || strlen(sentence) < 6) {
        return false;
    }
    
    // Format: $....*HH
    const char *star = strchr(sentence, '*');
    if (!star || star - sentence < 6) {
        return false;
    }
    
    // Calculer checksum
    uint8_t calc_sum = 0;
    for (const char *p = sentence + 1; p < star; p++) {
        calc_sum ^= *p;
    }
    
    // Lire checksum attendu
    char *end;
    uint8_t expected_sum = (uint8_t)strtol(star + 1, &end, 16);
    
    return calc_sum == expected_sum;
}

// ============================================================================
// TIMING
// ============================================================================

uint32_t utils_millis(void) {
    return millis();
}

void utils_delay(uint32_t ms) {
    delay(ms);
}
