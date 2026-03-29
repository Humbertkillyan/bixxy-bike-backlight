/**
 * @file gps.c
 * @brief Implémentation du module GPS
 * 
 * Responsabilités:
 * - Communication UART avec module GPS
 * - Parsing des sentences NMEA
 * - Validation et mise en cache des données
 */

#include "gps.h"
#include "../config.h"
#include "../utils/utils.h"
#include <string.h>

// ============================================================================
// CONSTANTES
// ============================================================================

#define GPS_BUFFER_SIZE         128
#define GPS_MAX_SENTENCE_LEN    82
#define GPS_PARSER_TIMEOUT_MS   2000

// ============================================================================
// TYPES PRIVÉS
// ============================================================================

typedef struct {
    bool initialized;
    bool has_fix;
    uint32_t last_read;
    uint8_t error_count;
    
    // Buffer de parsing
    char rx_buffer[GPS_BUFFER_SIZE];
    uint16_t rx_index;
} gps_state_t;

// ============================================================================
// VARIABLES PRIVÉES
// ============================================================================

static gps_state_t gps_state = {0};
static gps_data_t gps_cache = {0};

// ============================================================================
// FONCTIONS PRIVÉES
// ============================================================================

static bool _parse_nmea_sentence(const char *sentence, gps_data_t *data);
static bool _parse_rmc_sentence(const char *sentence, gps_data_t *data);
static bool _parse_gga_sentence(const char *sentence, gps_data_t *data);
static void _process_uart_data(void);
static bool _is_valid_sentence(const char *sentence);

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

bool gps_init(void) {
    if (gps_state.initialized) {
        return true;
    }
    
    utils_log("INFO", "Initializing GPS module");
    
    // Configurer UART pour GPS
    #if defined(PLATFORM_ESP32)
        Serial2.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
    #else
        Serial.begin(GPS_BAUD_RATE);  // Sur Uno/Mega, mapping dans Arduino
    #endif
    
    memset(&gps_state, 0, sizeof(gps_state));
    memset(&gps_cache, 0, sizeof(gps_cache));
    
    gps_state.initialized = true;
    gps_state.rx_index = 0;
    
    return true;
}

bool gps_read(gps_data_t *data) {
    if (!gps_state.initialized || !data) {
        return false;
    }
    
    // Lire les données disponibles
    _process_uart_data();
    
    // Copier les données en cache
    *data = gps_cache;
    
    return gps_state.has_fix;
}

bool gps_is_ready(void) {
    return gps_state.initialized && gps_state.has_fix;
}

uint8_t gps_get_satellites(void) {
    return gps_cache.satellites;
}

void gps_deinit(void) {
    if (gps_state.initialized) {
        utils_log("INFO", "Stopping GPS module");
    }
    gps_state.initialized = false;
}

// ============================================================================
// FONCTIONS PRIVÉES
// ============================================================================

static void _process_uart_data(void) {
    // Simple implementation - à améliorer selon besoin
    // En production, utiliser interrupt-driven UART
}

static bool _parse_nmea_sentence(const char *sentence, gps_data_t *data) {
    if (!sentence || !data || !_is_valid_sentence(sentence)) {
        return false;
    }
    
    // Dispatcher selon le type de sentence
    if (strstr(sentence, "$GPRMC") || strstr(sentence, "$GNRMC")) {
        return _parse_rmc_sentence(sentence, data);
    } else if (strstr(sentence, "$GPGGA") || strstr(sentence, "$GNGGA")) {
        return _parse_gga_sentence(sentence, data);
    }
    
    return false;
}

static bool _parse_rmc_sentence(const char *sentence, gps_data_t *data) {
    // TODO: Implémenter parsing NMEA RMC
    // Format: $GPRMC,time,status,lat,N/S,lon,E/W,speed,course,date,mv,E/W
    return false;
}

static bool _parse_gga_sentence(const char *sentence, gps_data_t *data) {
    // TODO: Implémenter parsing NMEA GGA
    // Format: $GPGGA,time,lat,N/S,lon,E/W,fix,sats,hdop,alt,M,geoid,M
    return false;
}

static bool _is_valid_sentence(const char *sentence) {
    if (!sentence || strlen(sentence) < 10) {
        return false;
    }
    
    // Vérifier checksum NMEA si présent
    // Format: $....*HH où HH est CheckSum
    
    return true;  // TODO: Implémenter validation complète
}
