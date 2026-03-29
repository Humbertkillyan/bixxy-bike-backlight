/**
 * @file accelerometer.c
 * @brief Implémentation du module accéléromètre
 * 
 * Responsabilités:
 * - Communication I2C avec MPU-6050
 * - Configuration du capteur
 * - Calibration et conversion des données
 * - Application de filtres si nécessaire
 */

#include "accelerometer.h"
#include "../config.h"
#include "../utils/utils.h"
#include <Wire.h>
#include <math.h>

// ============================================================================
// CONSTANTES REGISTRES MPU-6050
// ============================================================================

#define MPU6050_ADDR            0x68
#define MPU6050_ACCEL_XOUT_H    0x3B
#define MPU6050_PWR_MGMT_1      0x6B
#define MPU6050_CONFIG          0x1A
#define MPU6050_ACCEL_CONFIG    0x1C

// ============================================================================
// TYPES PRIVÉS
// ============================================================================

typedef struct {
    bool initialized;
    accel_scale_t current_scale;
    float scale_factor;
    int16_t offset_x, offset_y, offset_z;
    uint8_t error_count;
} accel_state_t;

// ============================================================================
// VARIABLES PRIVÉES
// ============================================================================

static accel_state_t accel_state = {0};

// ============================================================================
// FONCTIONS PRIVÉES
// ============================================================================

static bool _write_register(uint8_t reg, uint8_t value);
static bool _read_registers(uint8_t reg, uint8_t *buffer, uint8_t len);
static void _update_scale_factor(accel_scale_t scale);
static bool _self_test(void);

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

bool accelerometer_init(void) {
    if (accel_state.initialized) {
        return true;
    }
    
    utils_log("INFO", "Initializing Accelerometer (MPU-6050)");
    
    Wire.begin();
    Wire.setClock(I2C_SPEED);
    
    // Vérifier présence du capteur
    Wire.beginTransmission(MPU6050_ADDR);
    if (Wire.endTransmission() != 0) {
        utils_log("ERROR", "MPU-6050 not found on I2C bus");
        return false;
    }
    
    // Réveiller le capteur (sortie sleep mode)
    if (!_write_register(MPU6050_PWR_MGMT_1, 0x00)) {
        utils_log("ERROR", "Failed to wake MPU-6050");
        return false;
    }
    
    delay(100);  // Attendre stabilisation
    
    // Configuration par défaut
    if (!accelerometer_configure(ACCEL_SCALE_2G, ACCEL_RATE_50HZ)) {
        return false;
    }
    
    accel_state.initialized = true;
    accel_state.error_count = 0;
    memset(&accel_state.offset_x, 0, sizeof(int16_t) * 3);
    
    return true;
}

bool accelerometer_configure(accel_scale_t scale, accel_sample_rate_t rate) {
    if (!accel_state.initialized) {
        return false;
    }
    
    // Configurer l'échelle
    uint8_t accel_config = (scale << 3);
    if (!_write_register(MPU6050_ACCEL_CONFIG, accel_config)) {
        return false;
    }
    
    _update_scale_factor(scale);
    accel_state.current_scale = scale;
    
    return true;
}

bool accelerometer_read(accel_data_t *data) {
    if (!accel_state.initialized || !data) {
        return false;
    }
    
    // Lire les 6 registres accélération (3 axes × 2 bytes)
    uint8_t buffer[6];
    if (!_read_registers(MPU6050_ACCEL_XOUT_H, buffer, 6)) {
        accel_state.error_count++;
        return false;
    }
    
    // Combiner high et low bytes
    data->x_raw = ((int16_t)buffer[0] << 8) | buffer[1];
    data->y_raw = ((int16_t)buffer[2] << 8) | buffer[3];
    data->z_raw = ((int16_t)buffer[4] << 8) | buffer[5];
    
    // Appliquer offset calibration
    data->x_raw -= accel_state.offset_x;
    data->y_raw -= accel_state.offset_y;
    data->z_raw -= accel_state.offset_z;
    
    // Convertir en g
    data->x = (float)data->x_raw * accel_state.scale_factor;
    data->y = (float)data->y_raw * accel_state.scale_factor;
    data->z = (float)data->z_raw * accel_state.scale_factor;
    
    // Calculer magnitude
    data->magnitude = sqrt(data->x * data->x + 
                          data->y * data->y + 
                          data->z * data->z);
    
    data->timestamp = millis();
    accel_state.error_count = 0;
    
    return true;
}

bool accelerometer_calibrate(void) {
    if (!accel_state.initialized) {
        return false;
    }
    
    utils_log("INFO", "Calibrating accelerometer (keep level)");
    
    int32_t sum_x = 0, sum_y = 0, sum_z = 0;
    const int16_t samples = 50;
    uint8_t buffer[6];
    
    uint32_t deadline = millis() + 5000;  // 5 secondes max
    
    for (int16_t i = 0; i < samples && millis() < deadline; i++) {
        if (!_read_registers(MPU6050_ACCEL_XOUT_H, buffer, 6)) {
            continue;
        }
        
        sum_x += ((int16_t)buffer[0] << 8) | buffer[1];
        sum_y += ((int16_t)buffer[2] << 8) | buffer[3];
        sum_z += ((int16_t)buffer[4] << 8) | buffer[5];
        
        delay(50);
    }
    
    if (millis() >= deadline) {
        utils_log("WARN", "Calibration timeout");
        return false;
    }
    
    // Le capteur devrait lire (0,0,g) en conditions nominales
    accel_state.offset_x = sum_x / samples;
    accel_state.offset_y = sum_y / samples;
    accel_state.offset_z = (sum_z / samples) - 16384;  // 16384 LSB/g pour ±2g
    
    utils_log("INFO", "Calibration complete");
    return true;
}

bool accelerometer_is_moving(float threshold) {
    accel_data_t data;
    if (!accelerometer_read(&data)) {
        return false;
    }
    return data.magnitude > threshold;
}

void accelerometer_deinit(void) {
    if (accel_state.initialized) {
        utils_log("INFO", "Stopping accelerometer");
    }
    accel_state.initialized = false;
}

// ============================================================================
// FONCTIONS PRIVÉES
// ============================================================================

static bool _write_register(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.write(value);
    return Wire.endTransmission() == 0;
}

static bool _read_registers(uint8_t reg, uint8_t *buffer, uint8_t len) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    if (Wire.endTransmission() != 0) {
        return false;
    }
    
    Wire.requestFrom(MPU6050_ADDR, len);
    if (Wire.available() != len) {
        return false;
    }
    
    for (uint8_t i = 0; i < len; i++) {
        buffer[i] = Wire.read();
    }
    
    return true;
}

static void _update_scale_factor(accel_scale_t scale) {
    // Conversion: LSB to g selon la plage
    switch (scale) {
        case ACCEL_SCALE_2G:   accel_state.scale_factor = 1.0f / 16384.0f; break;
        case ACCEL_SCALE_4G:   accel_state.scale_factor = 1.0f / 8192.0f;  break;
        case ACCEL_SCALE_8G:   accel_state.scale_factor = 1.0f / 4096.0f;  break;
        case ACCEL_SCALE_16G:  accel_state.scale_factor = 1.0f / 2048.0f;  break;
    }
}

static bool _self_test(void) {
    // TODO: Implémenter self-test MPU-6050
    return true;
}
