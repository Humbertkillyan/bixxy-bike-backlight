/**
 * @file utils.h
 * @brief Utilitaires génériques partagés
 * @details Fonctions math, timing, logging et helpers
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// LOGGING
// ============================================================================

/**
 * @brief Logger un message
 * @param level Niveau: "ERROR", "WARN", "INFO", "DEBUG"
 * @param msg Message à logger
 */
void utils_log(const char *level, const char *msg);

/**
 * @brief Logger un message avec paramètre entier
 * @param level Niveau de log
 * @param msg Message
 * @param value Valeur entière
 */
void utils_log_int(const char *level, const char *msg, int value);

/**
 * @brief Logger un message avec paramètre float
 * @param level Niveau de log
 * @param msg Message
 * @param value Valeur float
 */
void utils_log_float(const char *level, const char *msg, float value);

// ============================================================================
// MATH
// ============================================================================

/**
 * @brief Mapper une valeur d'un range à un autre
 * @param x Valeur à mapper
 * @param in_min Min de l'intervalle d'entrée
 * @param in_max Max de l'intervalle d'entrée
 * @param out_min Min de l'intervalle de sortie
 * @param out_max Max de l'intervalle de sortie
 * @return Valeur mappée
 */
float utils_map(float x, float in_min, float in_max, 
                float out_min, float out_max);

/**
 * @brief Limiter une valeur dans un range
 * @param x Valeur
 * @param min Minimum
 * @param max Maximum
 * @return Valeur limitée
 */
float utils_constrain(float x, float min, float max);

/**
 * @brief Calculer distance entre deux coordonnées GPS (Haversine)
 * @param lat1 Latitude point 1
 * @param lon1 Longitude point 1
 * @param lat2 Latitude point 2
 * @param lon2 Longitude point 2
 * @return Distance en mètres
 */
float utils_distance_haversine(double lat1, double lon1,
                               double lat2, double lon2);

// ============================================================================
// STRING
// ============================================================================

/**
 * @brief Convertir float en string
 * @param value Valeur float
 * @param str Buffer de sortie
 * @param decimals Nombre de décimales
 * @note Le buffer doit être assez grand
 */
void utils_float_to_str(float value, char *str, int decimals);

/**
 * @brief Convertir entier en string
 * @param value Valeur entière
 * @param str Buffer de sortie
 * @param base Base (10, 16, etc)
 */
void utils_itoa(int value, char *str, int base);

// ============================================================================
// CRC & VALIDATION
// ============================================================================

/**
 * @brief Calculer CRC-8
 * @param data Données
 * @param len Longueur
 * @return CRC-8
 */
uint8_t utils_crc8(const uint8_t *data, size_t len);

/**
 * @brief Vérifier checksum NMEA
 * @param sentence Chaîne NMEA
 * @return true si checksum valide
 */
bool utils_validate_nmea(const char *sentence);

// ============================================================================
// TIMING
// ============================================================================

/**
 * @brief Obtenir le temps écoulé en ms depuis démarrage
 * @return Millisecondes
 */
uint32_t utils_millis(void);

/**
 * @brief Attendre N ms
 * @param ms Millisecondes
 * @note Non-bloquant si possible
 */
void utils_delay(uint32_t ms);

#endif // UTILS_H
