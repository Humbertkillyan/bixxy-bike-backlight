/**
 * @file gps.h
 * @brief Interface du module GPS
 * @details Gestion de la communication avec le module GPS et parsing des données NMEA
 */

#ifndef GPS_H
#define GPS_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// TYPES
// ============================================================================

/**
 * @brief Données GPS complètes
 */
typedef struct {
    double latitude;        ///< Latitude en degrés (N = +, S = -)
    double longitude;       ///< Longitude en degrés (E = +, W = -)
    float altitude;         ///< Altitude en mètres
    float speed;            ///< Vitesse en km/h
    float course;           ///< Direction de déplacement (0-359°)
    uint8_t satellites;     ///< Nombre de satellites visibles
    uint16_t hdop;          ///< Dilution horizontale (x100)
    uint32_t timestamp;     ///< Timestamp en ms depuis démarrage
    bool has_fix;           ///< True si fix obtenu
} gps_data_t;

typedef enum {
    GPS_NO_FIX = 0,
    GPS_FIX_2D = 2,
    GPS_FIX_3D = 3
} gps_fix_type_t;

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

/**
 * @brief Initialiser le module GPS
 * @details Configure le port UART et démarre la communication
 * @return true si succès, false sinon
 * @note À appeler une seule fois au démarrage
 */
bool gps_init(void);

/**
 * @brief Lire les données GPS disponibles
 * @param data Pointeur vers structure à remplir
 * @return true si nouvelle donnée disponible, false sinon
 * @note À appeler régulièrement pour traiter les données
 */
bool gps_read(gps_data_t *data);

/**
 * @brief Vérifier si le GPS a un fix valide
 * @return true si données valides, false sinon
 */
bool gps_is_ready(void);

/**
 * @brief Obtenir le nombre de satellites actuellement utilisés
 * @return Nombre de satellites pour le fix actuel
 */
uint8_t gps_get_satellites(void);

/**
 * @brief Arrêter le module GPS
 */
void gps_deinit(void);

#endif // GPS_H
