/**
 * @file leds.h
 * @brief Interface du module contrôle LEDs
 * @details Gestion des LEDs adressables (WS2812) ou GPIO simples
 */

#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// TYPES
// ============================================================================

typedef struct {
    uint8_t r, g, b;  ///< Composantes RGB (0-255)
} rgb_color_t;

typedef enum {
    LED_MODE_OFF = 0,        ///< Toutes LEDs éteintes
    LED_MODE_STATIC = 1,     ///< Couleur fixe
    LED_MODE_PULSE = 2,      ///< Pulsation lente
    LED_MODE_RAINBOW = 3,    ///< Arc-en-ciel rotatif
    LED_MODE_CHASE = 4,      ///< Poursuivant LED par LED
    LED_MODE_ADAPTIVE = 5    ///< Couleur adaptée selon contexte
} led_mode_t;

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

/**
 * @brief Initialiser le système LED
 * @details Configure les pins et initialize la bibliothèque
 * @return true si succès
 */
bool leds_init(void);

/**
 * @brief Configurer le mode d'affichage
 * @param mode Mode LED parmi led_mode_t
 * @return true si configuré
 */
bool leds_set_mode(led_mode_t mode);

/**
 * @brief Définir la couleur (utilisée selon mode)
 * @param r Composante rouge (0-255)
 * @param g Composante verte (0-255)
 * @param b Composante bleue (0-255)
 * @return true si OK
 */
bool leds_fill_color(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Contrôler la luminosité générale
 * @param brightness Valeur 0-255 (0=off, 255=max)
 * @return true si OK
 */
bool leds_set_brightness(uint8_t brightness);

/**
 * @brief Appliquer les changements aux LEDs
 * @details À appeler régulièrement (60 Hz recommandé)
 * @return true si OK
 */
bool leds_update(void);

/**
 * @brief Arrêter le système LED
 */
void leds_deinit(void);

#endif // LEDS_H
