/**
 * @file accelerometer.h
 * @brief Interface du module accéléromètre
 * @details Gestion de la communication I2C avec MPU-6050 et traitement des données
 */

#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// TYPES
// ============================================================================

/**
 * @brief Données accelerométriques complètes
 */
typedef struct {
    int16_t x_raw, y_raw, z_raw;   ///< Valeurs brutes (-32768 à 32767)
    float x, y, z;                  ///< Accélérations converties en g
    float magnitude;                ///< √(x² + y² + z²)
    uint32_t timestamp;             ///< Timestamp en ms
} accel_data_t;

typedef enum {
    ACCEL_SCALE_2G = 0,
    ACCEL_SCALE_4G = 1,
    ACCEL_SCALE_8G = 2,
    ACCEL_SCALE_16G = 3
} accel_scale_t;

typedef enum {
    ACCEL_RATE_10HZ = 0,
    ACCEL_RATE_50HZ = 1,
    ACCEL_RATE_100HZ = 2,
    ACCEL_RATE_200HZ = 3
} accel_sample_rate_t;

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

/**
 * @brief Initialiser le module accéléromètre
 * @details Configure I2C et le capteur MPU-6050
 * @return true si succès, false sinon
 */
bool accelerometer_init(void);

/**
 * @brief Configurer les paramètres de l'accéléromètre
 * @param scale Plage de mesure (±2g, ±4g, ±8g, ±16g)
 * @param rate Fréquence d'échantillonnage
 * @return true si configuré, false sinon
 */
bool accelerometer_configure(accel_scale_t scale, accel_sample_rate_t rate);

/**
 * @brief Lire les données accéléométriques
 * @param data Pointeur vers structure à remplir
 * @return true si lecture OK, false sinon
 */
bool accelerometer_read(accel_data_t *data);

/**
 * @brief Calibrer le capteur (offset et dynamique)
 * @details À appeler une seule fois au démarrage, sur surface plane
 * @return true si calibration OK, false si timeout
 * @note Dure environ 5 secondes
 */
bool accelerometer_calibrate(void);

/**
 * @brief Déterminer si mouvement sigificatif détecté
 * @param threshold Seuil de magnitude (en g)
 * @return true si magnitude > threshold
 */
bool accelerometer_is_moving(float threshold);

/**
 * @brief Arrêter le module accéléromètre
 */
void accelerometer_deinit(void);

#endif // ACCELEROMETER_H
