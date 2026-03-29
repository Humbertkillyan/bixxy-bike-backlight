/**
 * @file accelerometer.c
 * @brief Implémentation du module accéléromètre avec électroniccats MPU6050
 * 
 * Utilise la bibliothèque electroniccats/MPU6050 pour communication I2C robuste
 * - https://github.com/electroniccats/mpu6050
 */

#include "accelerometer.h"
#include "../config.h"
#include "../utils/utils.h"
#include <Wire.h>
#include <math.h>
#include <MPU6050.h>

// ============================================================================
// VARIABLES PRIVÉES
// ============================================================================

static MPU6050 mpu;
static bool is_initialized = false;
static accel_data_t last_data = {0};
static int16_t offset_x = 0, offset_y = 0, offset_z = 0;
static float scale_factor = 1.0f / 16384.0f;  // Default for ±2g

// ============================================================================
// FONCTIONS PRIVÉES
// ============================================================================

/**
 * @brief Mettre à jour le facteur d'échelle selon la plage configurée
 */
static void update_scale_factor(accel_scale_t scale) {
    switch (scale) {
        case ACCEL_SCALE_2G:   scale_factor = 1.0f / 16384.0f; break;
        case ACCEL_SCALE_4G:   scale_factor = 1.0f / 8192.0f;  break;
        case ACCEL_SCALE_8G:   scale_factor = 1.0f / 4096.0f;  break;
        case ACCEL_SCALE_16G:  scale_factor = 1.0f / 2048.0f;  break;
    }
}

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

bool accelerometer_init(void) {
    if (is_initialized) {
        return true;
    }
    
    utils_log("INFO", "Initializing MPU-6050 Accelerometer");
    
    // Initialiser I2C
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, I2C_SPEED);
    
    // Initialiser la bibliothèque MPU6050 à l'adresse configurée
    mpu.initialize(MPU6050_I2C_ADDR);
    
    // Vérifier la connexion
    if (!mpu.testConnection()) {
        utils_log_int("ERROR", "MPU-6050 not responding at address", MPU6050_I2C_ADDR);
        return false;
    }
    
    utils_log("INFO", "MPU-6050 detected successfully");
    
    // Configuration initiale
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);  // ±2g par défaut
    mpu.setDLPFMode(MPU6050_DLPF_CFG);               // Low-pass filter
    mpu.setSleepEnabled(false);
    mpu.setClockSource(MPU6050_CLOCK_PLL_XGYRO);  // Horloge interne
    
    is_initialized = true;
    utils_log("INFO", "MPU-6050 initialization complete");
    
    return true;
}

bool accelerometer_configure(accel_scale_t scale, accel_sample_rate_t rate) {
    if (!is_initialized) {
        return false;
    }
    
    // Configurer l'échelle d'accélération
    uint8_t fs_range;
    switch (scale) {
        case ACCEL_SCALE_2G:   fs_range = MPU6050_ACCEL_FS_2;   break;
        case ACCEL_SCALE_4G:   fs_range = MPU6050_ACCEL_FS_4;   break;
        case ACCEL_SCALE_8G:   fs_range = MPU6050_ACCEL_FS_8;   break;
        case ACCEL_SCALE_16G:  fs_range = MPU6050_ACCEL_FS_16;  break;
        default:               fs_range = MPU6050_ACCEL_FS_2;   break;
    }
    
    mpu.setFullScaleAccelRange(fs_range);
    update_scale_factor(scale);
    
    // Configurer la fréquence d'échantillonnage (SMPLRT_DIV = 1000 / (1 + divider) Hz)
    uint8_t divider;
    switch (rate) {
        case ACCEL_RATE_10HZ:   divider = 99;   break;   // ~10 Hz
        case ACCEL_RATE_50HZ:   divider = 19;   break;   // ~50 Hz
        case ACCEL_RATE_100HZ:  divider = 9;    break;   // ~100 Hz
        case ACCEL_RATE_200HZ:  divider = 4;    break;   // ~200 Hz
        default:                divider = 19;   break;
    }
    
    mpu.setRate(divider);
    
    utils_log("INFO", "Accelerometer configured");
    return true;
}

bool accelerometer_read(accel_data_t *data) {
    if (!is_initialized || !data) {
        return false;
    }
    
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);
    
    // Appliquer les offsets de calibration
    ax -= offset_x;
    ay -= offset_y;
    az -= offset_z;
    
    // Stocker valeurs brutes et converties
    data->x_raw = ax;
    data->y_raw = ay;
    data->z_raw = az;
    
    data->x = ax * scale_factor;
    data->y = ay * scale_factor;
    data->z = az * scale_factor;
    
    // Calculer la magnitude
    data->magnitude = sqrtf(
        data->x * data->x +
        data->y * data->y +
        data->z * data->z
    );
    
    data->timestamp = utils_millis();
    last_data = *data;
    
    return true;
}

bool accelerometer_calibrate(void) {
    if (!is_initialized) {
        return false;
    }
    
    utils_log("INFO", "Starting calibration - keep device level");
    
    int32_t sum_x = 0, sum_y = 0, sum_z = 0;
    const int16_t num_samples = 100;
    const uint32_t sample_delay_ms = 50;
    
    utils_log_int("INFO", "Taking samples", num_samples);
    
    // Collecter les échantillons
    for (int16_t i = 0; i < num_samples; i++) {
        int16_t ax, ay, az;
        mpu.getAcceleration(&ax, &ay, &az);
        
        sum_x += ax;
        sum_y += ay;
        sum_z += az;
        
        utils_delay(sample_delay_ms);
    }
    
    // Calculer les offsets
    // Le capteur doit lire environ (0, 0, 1g) si placé à plat
    offset_x = sum_x / num_samples;
    offset_y = sum_y / num_samples;
    offset_z = (sum_z / num_samples) - (int16_t)(1.0f / scale_factor);  // Soustraire 1g
    
    utils_log_int("INFO", "Offset X", offset_x);
    utils_log_int("INFO", "Offset Y", offset_y);
    utils_log_int("INFO", "Offset Z", offset_z);
    
    utils_log("INFO", "Calibration complete!");
    return true;
}

bool accelerometer_is_moving(float threshold) {
    return last_data.magnitude > threshold;
}

void accelerometer_deinit(void) {
    if (is_initialized) {
        mpu.setSleepEnabled(true);
        is_initialized = false;
        utils_log("INFO", "Accelerometer deinitialized");
    }
}
