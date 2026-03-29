/**
 * @file gps.c
 * @brief Implémentation du module GPS avec TinyGPSPlus
 * @details Parsing NMEA pour Beitian 220T via UART1
 * - Bibliothèque: TinyGPSPlus
 * - Module: Beitian 220T (9600 baud NMEA)
 */

#include "gps.h"
#include "../config.h"
#include "../utils/utils.h"
#include <string.h>
#include <TinyGPS++.h>

// ============================================================================
// VARIABLES PRIVÉES
// ============================================================================

static TinyGPSPlus gps_parser;
static bool is_initialized = false;
static gps_data_t latest_data = {0};
static uint32_t last_update = 0;
static uint32_t last_location_update = 0;
static uint8_t chars_processed = 0;
static uint8_t failed_checksums = 0;

// Sélectionner le bon port UART selon la plateforme
#ifdef PLATFORM_ESP32_NANO
    #define GPS_SERIAL Serial1  // UART1 pour Beitian 220T
#else
    #define GPS_SERIAL Serial   // Port série principal
#endif

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

bool gps_init(void) {
    if (is_initialized) {
        return true;
    }
    
    utils_log("INFO", "Initializing Beitian 220T GPS module");
    
    // Configurer le port UART pour GPS
    GPS_SERIAL.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
    
    if (!GPS_SERIAL) {
        utils_log("ERROR", "Failed to initialize GPS UART");
        return false;
    }
    
    is_initialized = true;
    last_update = utils_millis();
    
    utils_log("INFO", "Beitian 220T GPS initialized (9600 baud)");
    return true;
}

bool gps_read(gps_data_t *data) {
    if (!is_initialized || !data) {
        return false;
    }
    
    // Traiter les données disponibles sur UART
    while (GPS_SERIAL.available() > 0) {
        char c = GPS_SERIAL.read();
        if (gps_parser.encode(c)) {
            // Une phrase complète a été parsée
            chars_processed++;
            
            // Mise à jour des données si nouvelle localisation
            if (gps_parser.location.isUpdated()) {
                last_location_update = utils_millis();
                
                latest_data.latitude = gps_parser.location.lat();
                latest_data.longitude = gps_parser.location.lng();
                latest_data.altitude = gps_parser.altitude.meters();
                latest_data.speed = gps_parser.speed.kmph();
                latest_data.course = gps_parser.course.deg();
                latest_data.satellites = gps_parser.satellites.value();
                latest_data.hdop = gps_parser.hdop.value();
                latest_data.has_fix = gps_parser.location.isValid();
                latest_data.timestamp = utils_millis();
            }
        }
    }
    
    // Copier les données mises en cache
    *data = latest_data;
    
    // Retourner vrai si on a une fix valide
    return gps_parser.location.isValid() && gps_parser.location.age() < 2000;
}

bool gps_is_ready(void) {
    if (!is_initialized) {
        return false;
    }
    
    // Check if fix is valid and not too old
    return gps_parser.location.isValid() && 
           (utils_millis() - last_location_update) < GPS_TIMEOUT_MS;
}

uint8_t gps_get_satellites(void) {
    return gps_parser.satellites.value();
}

void gps_deinit(void) {
    if (is_initialized) {
        GPS_SERIAL.end();
        is_initialized = false;
        utils_log("INFO", "GPS module stopped");
    }
}

