/**
 * @file power.h
 * @brief Interface du module de gestion d'énergie (INA219 Wattmètre)
 * @details Mesure de tension, courant et puissance via I2C
 * - https://github.com/DFRobot/DFRobot_INA219
 */

#ifndef POWER_H
#define POWER_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// TYPES
// ============================================================================

/**
 * @brief Mesures de puissance complètes
 */
typedef struct {
    float bus_voltage;          ///< Tension d'alimentation (V)
    float shunt_voltage;        ///< Tension aux bornes du shunt (mV)
    float current_ma;           ///< Courant (mA)
    float power_mw;             ///< Puissance (mW)
    uint32_t timestamp;         ///< Timestamp en ms
} power_data_t;

/**
 * @brief Plages de mesure disponibles
 */
typedef enum {
    POWER_RANGE_16V_400MA = 0,  ///< 16V, 400mA max
    POWER_RANGE_32V_800MA = 1,  ///< 32V max (si disponible)
} power_range_t;

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

/**
 * @brief Initialiser le module de gestion d'énergie
 * @details Configure I2C et le capteur INA219 (adresse 0x40 par défaut)
 * @return true si succès, false sinon
 */
bool power_init(void);

/**
 * @brief Configurer la plage de mesure
 * @param range Plage (16V/32V selon le modèle)
 * @return true si configuré, false sinon
 */
bool power_configure(power_range_t range);

/**
 * @brief Lire les mesures d'énergie
 * @param data Pointeur vers structure à remplir
 * @return true si lecture OK, false sinon
 */
bool power_read(power_data_t *data);

/**
 * @brief Lire la tension d'alimentation seule
 * @return Tension en volts (0 si erreur)
 */
float power_get_bus_voltage(void);

/**
 * @brief Lire le courant seul
 * @return Courant en milliampères (0 si erreur)
 */
float power_get_current_ma(void);

/**
 * @brief Lire la puissance seule
 * @return Puissance en milliwatts (0 si erreur)
 */
float power_get_power_mw(void);

/**
 * @brief Déterminer si la batterie est faible
 * @param min_voltage Seuil minimum en volts
 * @return true si batterie < min_voltage
 */
bool power_is_low_battery(float min_voltage);

/**
 * @brief Réinitialiser l'INA219 (clear errors)
 */
void power_reset(void);

/**
 * @brief Dés-initialiser le module
 */
void power_deinit(void);

#endif // POWER_H
