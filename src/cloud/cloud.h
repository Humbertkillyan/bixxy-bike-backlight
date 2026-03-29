/**
 * @file cloud.h
 * @brief Interface du module intégration cloud
 * @details Communication WiFi et sync des données vers backend cloud
 */

#ifndef CLOUD_H
#define CLOUD_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// TYPES
// ============================================================================

/**
 * @brief Payload de données à envoyer vers le cloud
 */
typedef struct {
    uint32_t timestamp;      ///< Timestamp en ms
    double latitude;         ///< Position GPS
    double longitude;        ///< Position GPS
    float speed;            ///< Vitesse km/h
    uint8_t battery;        ///< Niveau batterie 0-100%
} cloud_payload_t;

typedef enum {
    CLOUD_STATE_DISCONNECTED = 0,
    CLOUD_STATE_CONNECTING = 1,
    CLOUD_STATE_CONNECTED = 2,
    CLOUD_STATE_ERROR = 3
} cloud_state_t;

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

/**
 * @brief Initialiser le module cloud
 * @param ssid SSID du réseau WiFi
 * @param password Mot de passe WiFi
 * @return true si initialization OK
 */
bool cloud_init(const char *ssid, const char *password);

/**
 * @brief Obtenir l'état de connexion
 * @return État actuel de la connexion cloud
 */
cloud_state_t cloud_get_state(void);

/**
 * @brief Envoyer des données vers le cloud
 * @param data Pointeur vers structure de données
 * @return true si envoi OK, false sinon
 * @note Non-bloquant (retourne immédiatement)
 */
bool cloud_send_data(const cloud_payload_t *data);

/**
 * @brief Recevoir une commande du cloud
 * @param buffer Buffer pour la commande reçue
 * @param len Taille max du buffer
 * @return true si commande reçue, false sinon
 * @note Non-bloquant
 */
bool cloud_receive_command(char *buffer, size_t len);

/**
 * @brief Arrêter la connexion cloud
 */
void cloud_deinit(void);

#endif // CLOUD_H
