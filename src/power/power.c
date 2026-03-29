/**
 * @file power.c
 * @brief Implémentation du module INA219 (Wattmètre I2C)
 * @details Mesure de tension, courant et puissance
 * - Bibliothèque: DFRobot/DFRobot_INA219
 */

#include "power.h"
#include "../config.h"
#include "../utils/utils.h"
#include <Wire.h>
#include <DFRobot_INA219.h>

// ============================================================================
// VARIABLES PRIVÉES
// ============================================================================

static DFRobot_INA219 ina219;
static bool is_initialized = false;
static power_data_t last_data = {0};

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

bool power_init(void) {
    if (is_initialized) {
        return true;
    }
    
    utils_log("INFO", "Initializing INA219 Power Monitor");
    
    // Initialiser I2C
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, I2C_SPEED);
    
    // Initialiser l'INA219 à l'adresse configurée
    if (!ina219.init(INA219_I2C_ADDR)) {
        utils_log_int("ERROR", "INA219 failed to initialize at address", INA219_I2C_ADDR);
        return false;
    }
    
    utils_log("INFO", "INA219 detected successfully");
    
    // Configuration du shunt resistance
    // INA219 sait normalement détecter le shunt, mais on peut le spécifier
    ina219.setShuntRes(INA219_SHUNT_OHMS);
    
    // Configuration de la calibration
    // Pour une meilleure précision avec notre shunt de 0.1 Ohm
    ina219.linearCalibrate(INA219_MAX_AMPS);
    
    is_initialized = true;
    utils_log("INFO", "INA219 Power Monitor initialized");
    
    return true;
}

bool power_configure(power_range_t range) {
    if (!is_initialized) {
        return false;
    }
    
    // Configuration de la plage de tension (sur INA219, c'est généralement auto-détecté)
    // Cette fonction est un placeholder pour compatibilité API
    utils_log("INFO", "Power monitor range configured");
    return true;
}

bool power_read(power_data_t *data) {
    if (!is_initialized || !data) {
        return false;
    }
    
    // Lire les valeurs de l'INA219
    data->bus_voltage = ina219.getBusVoltage_V();      // Tension d'alimentation en V
    data->shunt_voltage = ina219.getShuntVoltage_mV();  // Tension du shunt en mV
    data->current_ma = ina219.getCurrent_mA();          // Courant en mA
    data->power_mw = ina219.getPower_mW();              // Puissance en mW
    
    data->timestamp = utils_millis();
    last_data = *data;
    
    return true;
}

float power_get_bus_voltage(void) {
    if (!is_initialized) {
        return 0.0f;
    }
    return ina219.getBusVoltage_V();
}

float power_get_current_ma(void) {
    if (!is_initialized) {
        return 0.0f;
    }
    return ina219.getCurrent_mA();
}

float power_get_power_mw(void) {
    if (!is_initialized) {
        return 0.0f;
    }
    return ina219.getPower_mW();
}

bool power_is_low_battery(float min_voltage) {
    if (!is_initialized) {
        return false;
    }
    float v = ina219.getBusVoltage_V();
    return v < min_voltage;
}

void power_reset(void) {
    if (!is_initialized) {
        return;
    }
    // Réinitialiser l'appareil
    ina219.begin();
    utils_log("INFO", "INA219 reset");
}

void power_deinit(void) {
    if (is_initialized) {
        is_initialized = false;
        utils_log("INFO", "Power Monitor deinitialized");
    }
}
