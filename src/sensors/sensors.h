/**
 * @file sensors.h
 * @brief Interface du module fusion de capteurs
 * @details Agrégation et fusion des données de tous les capteurs
 */

#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>
#include <stdbool.h>
#include "../gps/gps.h"
#include "../accelerometer/accelerometer.h"

// ============================================================================
// TYPES
// ============================================================================

/**
 * @brief Données fusionnées de tous les capteurs
 */
typedef struct {
    gps_data_t gps;
    accel_data_t accel;
    float battery_voltage;
    float temperature;
    uint32_t uptime;
    bool is_moving;
    float ground_speed;
} sensor_fusion_t;

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

/**
 * @brief Initialiser tous les capteurs
 * @return true si tous les capteurs OK
 */
bool sensors_init(void);

/**
 * @brief Mettre à jour les données de tous les capteurs
 * @param fused Structure à remplir avec données fusionnées
 * @return true si mise à jour OK
 */
bool sensors_update_all(sensor_fusion_t *fused);

/**
 * @brief Obtenir la vitesse actuelle fusionnée
 * @param speed Pointeur vers résultat en km/h
 * @return true si disponible
 */
bool sensors_get_speed(float *speed);

/**
 * @brief Vérifier si vélo en mouvement
 * @return true si mouvement détecté
 */
bool sensors_is_moving(void);

/**
 * @brief Arrêter tous les capteurs
 */
void sensors_deinit(void);

#endif // SENSORS_H
