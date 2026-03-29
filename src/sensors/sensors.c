/**
 * @file sensors.c
 * @brief Implémentation du module fusion de capteurs
 */

#include "sensors.h"
#include "../config.h"
#include "../utils/utils.h"

// ============================================================================
// TYPES PRIVÉS
// ============================================================================

typedef struct {
    bool initialized;
    uint32_t last_update;
    float moving_threshold;  // g
} sensors_state_t;

// ============================================================================
// VARIABLES PRIVÉES
// ============================================================================

static sensors_state_t sensors_state = {0};
static sensor_fusion_t last_fused = {0};

// ============================================================================
// FONCTIONS PRIVÉES
// ============================================================================

static float _read_battery_voltage(void);
static float _read_temperature(void);

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

bool sensors_init(void) {
    if (sensors_state.initialized) {
        return true;
    }
    
    utils_log("INFO", "Initializing Sensor Fusion");
    
    sensors_state.initialized = true;
    sensors_state.last_update = millis();
    sensors_state.moving_threshold = 1.5f;  // g
    
    memset(&last_fused, 0, sizeof(last_fused));
    
    return true;
}

bool sensors_update_all(sensor_fusion_t *fused) {
    if (!sensors_state.initialized || !fused) {
        return false;
    }
    
    uint32_t now = millis();
    
    // Lire GPS
    gps_read(&fused->gps);
    
    // Lire accéléromètre
    accelerometer_read(&fused->accel);
    
    // Lire batterie
    fused->battery_voltage = _read_battery_voltage();
    
    // Lire température (optionnel)
    #if FEATURE_BATTERY_MONITOR
    fused->temperature = _read_temperature();
    #endif
    
    // Calculer uptime
    fused->uptime = now;
    
    // Fusionner vitesse: priorité GPS sinon accéléromètre
    if (fused->gps.has_fix && fused->gps.speed > 0) {
        fused->ground_speed = fused->gps.speed;
    } else {
        // Peut estimer de l'accéléromètre si nécessaire
        fused->ground_speed = 0;
    }
    
    // Déterminer si en mouvement
    fused->is_moving = (fused->accel.magnitude > sensors_state.moving_threshold) ||
                       (fused->ground_speed > 1.0f);
    
    // Mise en cache
    last_fused = *fused;
    sensors_state.last_update = now;
    
    return true;
}

bool sensors_get_speed(float *speed) {
    if (!speed) {
        return false;
    }
    *speed = last_fused.ground_speed;
    return true;
}

bool sensors_is_moving(void) {
    return last_fused.is_moving;
}

void sensors_deinit(void) {
    if (sensors_state.initialized) {
        utils_log("INFO", "Stopping Sensor Fusion");
    }
    sensors_state.initialized = false;
}

// ============================================================================
// FONCTIONS PRIVÉES
// ============================================================================

static float _read_battery_voltage(void) {
    #if FEATURE_BATTERY_MONITOR && PLATFORM_ARDUINO_UNO
    // Lire ADC et convertir
    int adc_value = analogRead(BATTERY_SENSE_PIN);
    float voltage = (adc_value / 1023.0f) * 5.0f * BATTERY_VOLTAGE_DIVIDER;
    return voltage;
    #else
    return 4.0f;  // Placeholder
    #endif
}

static float _read_temperature(void) {
    // TODO: Implémenter lecture température
    return 25.0f;  // Placeholder
}
