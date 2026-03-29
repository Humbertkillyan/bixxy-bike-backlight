/**
 * @file leds.c
 * @brief Implémentation du module LEDs
 */

#include "leds.h"
#include "../config.h"
#include "../utils/utils.h"

// ============================================================================
// TYPES PRIVÉS
// ============================================================================

typedef struct {
    bool initialized;
    led_mode_t current_mode;
    rgb_color_t current_color;
    uint8_t brightness;
    uint32_t animation_timer;
    uint8_t animation_index;
} leds_state_t;

// ============================================================================
// VARIABLES PRIVÉES
// ============================================================================

static leds_state_t leds_state = {0};

// ============================================================================
// FONCTIONS PRIVÉES
// ============================================================================

static void _update_mode_off(void);
static void _update_mode_static(void);
static void _update_mode_pulse(void);
static void _update_mode_rainbow(void);
static void _update_mode_chase(void);
static void _update_mode_adaptive(void);
static void _write_to_hardware(rgb_color_t *color);

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

bool leds_init(void) {
    if (leds_state.initialized) {
        return true;
    }
    
    utils_log("INFO", "Initializing LED system");
    
    // Configurer pin LED
    pinMode(LED_DATA_PIN, OUTPUT);
    digitalWrite(LED_DATA_PIN, LOW);
    
    // Initialiser state
    leds_state.initialized = true;
    leds_state.current_mode = LED_MODE_OFF;
    leds_state.brightness = LED_BRIGHTNESS_DEFAULT;
    leds_state.current_color = {0, 0, 0};
    leds_state.animation_timer = millis();
    leds_state.animation_index = 0;
    
    return true;
}

bool leds_set_mode(led_mode_t mode) {
    if (!leds_state.initialized || mode > LED_MODE_ADAPTIVE) {
        return false;
    }
    
    if (leds_state.current_mode != mode) {
        leds_state.current_mode = mode;
        leds_state.animation_index = 0;
        leds_state.animation_timer = millis();
    }
    
    return true;
}

bool leds_fill_color(uint8_t r, uint8_t g, uint8_t b) {
    if (!leds_state.initialized) {
        return false;
    }
    
    leds_state.current_color.r = r;
    leds_state.current_color.g = g;
    leds_state.current_color.b = b;
    
    return true;
}

bool leds_set_brightness(uint8_t brightness) {
    if (!leds_state.initialized) {
        return false;
    }
    
    leds_state.brightness = brightness;
    return true;
}

bool leds_update(void) {
    if (!leds_state.initialized) {
        return false;
    }
    
    // Dispatcher selon le mode
    switch (leds_state.current_mode) {
        case LED_MODE_OFF:
            _update_mode_off();
            break;
        case LED_MODE_STATIC:
            _update_mode_static();
            break;
        case LED_MODE_PULSE:
            _update_mode_pulse();
            break;
        case LED_MODE_RAINBOW:
            _update_mode_rainbow();
            break;
        case LED_MODE_CHASE:
            _update_mode_chase();
            break;
        case LED_MODE_ADAPTIVE:
            _update_mode_adaptive();
            break;
        default:
            return false;
    }
    
    return true;
}

void leds_deinit(void) {
    if (leds_state.initialized) {
        utils_log("INFO", "Stopping LED system");
        rgb_color_t off = {0, 0, 0};
        _write_to_hardware(&off);
    }
    
    leds_state.initialized = false;
}

// ============================================================================
// FONCTIONS PRIVÉES - MODES
// ============================================================================

static void _update_mode_off(void) {
    rgb_color_t off = {0, 0, 0};
    _write_to_hardware(&off);
}

static void _update_mode_static(void) {
    rgb_color_t color = leds_state.current_color;
    
    // Appliquer brightness
    color.r = (uint8_t)((color.r * leds_state.brightness) / 255);
    color.g = (uint8_t)((color.g * leds_state.brightness) / 255);
    color.b = (uint8_t)((color.b * leds_state.brightness) / 255);
    
    _write_to_hardware(&color);
}

static void _update_mode_pulse(void) {
    uint32_t now = millis();
    uint32_t elapsed = now - leds_state.animation_timer;
    
    // Période: 2 secondes
    float phase = fmod(elapsed / 1000.0f, 2.0f);
    if (phase > 1.0f) phase = 2.0f - phase;
    
    rgb_color_t color = leds_state.current_color;
    uint8_t level = (uint8_t)(phase * leds_state.brightness);
    
    color.r = (uint8_t)((color.r * level) / 255);
    color.g = (uint8_t)((color.g * level) / 255);
    color.b = (uint8_t)((color.b * level) / 255);
    
    _write_to_hardware(&color);
}

static void _update_mode_rainbow(void) {
    // TODO: Implémenter rainbow
    _update_mode_static();
}

static void _update_mode_chase(void) {
    // TODO: Implémenter chase
    _update_mode_static();
}

static void _update_mode_adaptive(void) {
    // TODO: Implémenter adaptive (basé sur capteurs)
    _update_mode_static();
}

static void _write_to_hardware(rgb_color_t *color) {
    // Simple PWM pour LED unique
    // TODO: Implémenter pour WS2812B ou GPIO
    
    #if LED_TYPE == LED_TYPE_GPIO
        // Écrire sur GPIO
        analogWrite(LED_DATA_PIN, color->r);  // Ou combine RGB
    #else
        // Écrire sur NeoPixel
        // TODO: Utiliser librairie Adafruit_NeoPixel
    #endif
}

#include <math.h>  // Pour fmod
