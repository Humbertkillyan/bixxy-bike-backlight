/**
 * @file main.c
 * @brief Implémentation du main et orchestration système
 * 
 * Responsabilités:
 * - Initialisation de tous les modules au démarrage
 * - Boucle principale orchestrant les modules
 * - Gestion des états globaux
 * - Synchronisation des modules
 */

#include "main.h"
#include "config.h"
#include "utils/utils.h"
#include "gps/gps.h"
#include "accelerometer/accelerometer.h"
#include "power/power.h"
#include "leds/leds.h"
#include "sensors/sensors.h"
#include "cloud/cloud.h"

// ============================================================================
// CONSTANTES
// ============================================================================

#define SYSTEM_LOOP_INTERVAL_MS   100  // Boucle à 10 Hz
#define LED_UPDATE_INTERVAL_MS    16   // LEDs à 60 Hz (~16ms)
#define CLOUD_SYNC_INTERVAL_MS    10000 // Sync cloud tous les 10s

// ============================================================================
// TYPES PRIVÉS
// ============================================================================

typedef struct {
    bool initialized;
    uint32_t last_loop;
    uint32_t last_led_update;
    uint32_t last_cloud_sync;
    uint32_t loop_count;
} system_state_t;

// ============================================================================
// VARIABLES PRIVÉES
// ============================================================================

static system_state_t system_state = {0};

// ============================================================================
// ARDUINO SETUP/LOOP
// ============================================================================

void setup(void) {
    // Initialiser la communication série
    Serial.begin(115200);
    
    // Petit délai pour stabilisation
    delay(500);
    
    Serial.println("\n\n===========================================");
    Serial.println("BIXXY Bike Backlight - Sys Init");
    Serial.println("===========================================\n");
    
    if (!system_init()) {
        Serial.println("ERROR: System init failed!");
        while (1) {
            delay(1000);
        }
    }
    
    Serial.println("\nSystem ready!\n");
}

void loop(void) {
    system_loop();
}

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

bool system_init(void) {
    if (system_state.initialized) {
        return true;
    }
    
    Serial.println("[MAIN] Initializing modules...");
    
    // Initialiser Power Monitor en premier (INA219)
    Serial.print("[MAIN] Init Power Monitor... ");
    if (!power_init()) {
        Serial.println("FAILED");
        return false;
    }
    Serial.println("OK");
    
    // Initialiser Accéléromètre (MPU-6050)
    Serial.print("[MAIN] Init Accelerometer... ");
    if (!accelerometer_init()) {
        Serial.println("FAILED");
        return false;
    }
    Serial.println("OK");
    
    // Calibrer l'accéléromètre
    Serial.print("[MAIN] Calibrating Accelerometer... ");
    if (!accelerometer_calibrate()) {
        Serial.println("TIMEOUT");
        // Continuer même si calibration timeout
    } else {
        Serial.println("OK");
    }
    
    // Initialiser GPS (Beitian 220T)
    Serial.print("[MAIN] Init GPS... ");
    if (!gps_init()) {
        Serial.println("FAILED");
        return false;
    }
    Serial.println("OK");
    
    // Initialiser LEDs
    Serial.print("[MAIN] Init LEDs... ");
    if (!leds_init()) {
        Serial.println("FAILED");
        return false;
    }
    Serial.println("OK");
    
    // Initialiser Capteurs (fusion)
    Serial.print("[MAIN] Init Sensors... ");
    if (!sensors_init()) {
        Serial.println("FAILED");
        return false;
    }
    Serial.println("OK");
    
    // Initialiser Cloud
    Serial.print("[MAIN] Init Cloud... ");
    if (!cloud_init(CLOUD_WIFI_SSID, CLOUD_WIFI_PASSWORD)) {
        Serial.println("WARNING: Cloud not ready (non-critical)");
    } else {
        Serial.println("OK");
    }
    
    system_state.initialized = true;
    system_state.last_loop = millis();
    system_state.last_led_update = millis();
    system_state.last_cloud_sync = millis();
    system_state.loop_count = 0;
    
    return true;
}

void system_loop(void) {
    if (!system_state.initialized) {
        return;
    }
    
    uint32_t now = millis();
    system_state.loop_count++;
    
    // ========================================================================
    // CAPTEURS - Toujours lire
    // ========================================================================
    
    sensor_fusion_t fused;
    sensors_update_all(&fused);
    
    // Optionnel: Debug log
    if (system_state.loop_count % 100 == 0) {
        Serial.print("[MAIN] Loop #");
        Serial.print(system_state.loop_count);
        Serial.print(" - GPS: ");
        Serial.print(fused.gps.latitude, 6);
        Serial.print(", ");
        Serial.print(fused.gps.longitude, 6);
        Serial.print(" | Speed: ");
        Serial.print(fused.ground_speed);
        Serial.println(" km/h");
    }
    
    // ========================================================================
    // LEDs - Update à fréquence régulière
    // ========================================================================
    
    if (now - system_state.last_led_update >= LED_UPDATE_INTERVAL_MS) {
        // Décider du mode LED basé sur l'état
        if (sensors_is_moving()) {
            // Mode adaptatif basé sur vitesse
            leds_set_mode(LED_MODE_ADAPTIVE);
            
            // Couleur basée sur vitesse
            if (fused.ground_speed < 5) {
                leds_fill_color(0, 255, 0);    // Vert - lent
            } else if (fused.ground_speed < 15) {
                leds_fill_color(255, 200, 0);  // Orange - moyen
            } else {
                leds_fill_color(255, 0, 0);    // Rouge - rapide
            }
        } else {
            // Pas de mouvement - LED off
            leds_set_mode(LED_MODE_OFF);
        }
        
        // Contrôler la luminosité selon batterie
        uint8_t brightness = fused.battery_voltage < 3.0f ? 50 : 255;
        leds_set_brightness(brightness);
        
        // Mettre à jour les LEDs
        leds_update();
        
        system_state.last_led_update = now;
    }
    
    // ========================================================================
    // CLOUD - Sync périodiquement
    // ========================================================================
    
    if (now - system_state.last_cloud_sync >= CLOUD_SYNC_INTERVAL_MS) {
        if (cloud_get_state() == CLOUD_STATE_CONNECTED) {
            // Préparer payload
            cloud_payload_t payload = {
                .timestamp = now,
                .latitude = fused.gps.latitude,
                .longitude = fused.gps.longitude,
                .speed = fused.ground_speed,
                .battery = (uint8_t)(fused.battery_voltage * 50)  // Convertir en %
            };
            
            // Envoyer
            if (cloud_send_data(&payload)) {
                Serial.println("[CLOUD] Data sent successfully");
            } else {
                Serial.println("[CLOUD] Send failed");
            }
        }
        
        system_state.last_cloud_sync = now;
    }
    
    // ========================================================================
    // Minimum delay pour ne pas surcharger
    // ========================================================================
    
    delay(10);  // 10ms minimum entre iterations
}

void system_shutdown(void) {
    if (!system_state.initialized) {
        return;
    }
    
    Serial.println("\n[MAIN] Shutting down...");
    
    leds_set_mode(LED_MODE_OFF);
    leds_update();
    
    cloud_deinit();
    sensors_deinit();
    leds_deinit();
    accelerometer_deinit();
    gps_deinit();
    
    system_state.initialized = false;
    
    Serial.println("[MAIN] Shutdown complete");
}

// ============================================================================
// Méthode alternative pour non-PlatformIO (Arduino IDE)
// Garder juste les setup/loop et enlever system init/loop
// ============================================================================
