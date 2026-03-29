/**
 * @file config.h
 * @brief Configuration centralisée du projet BIXXY
 * 
 * Tous les paramètres configurables du projet:
 * - Sélection matériel (Uno/Mega/ESP32)
 * - Pin definitions
 * - Paramètres des modules
 * - WiFi/Cloud
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// SÉLECTION DE LA PLATEFORME
// ============================================================================

// #define PLATFORM_ARDUINO_UNO    0
// #define PLATFORM_ARDUINO_MEGA   0
#define PLATFORM_ESP32_NANO     1  // ESP32 Nano (BIXXY project)

// ============================================================================
// PIN DEFINITIONS - ARDUINO UNO
// ============================================================================

#if defined(PLATFORM_ARDUINO_UNO)
    // GPS Module (UART soft ou hard)
    #define GPS_RX_PIN              0   // RX
    #define GPS_TX_PIN              1   // TX
    #define GPS_BAUD_RATE           9600
    
    // I2C - Accéléromètre
    #define I2C_SDA_PIN             A4  // SDA
    #define I2C_SCL_PIN             A5  // SCL
    #define I2C_SPEED               400000  // 400 kHz
    
    // LED Control
    #define LED_DATA_PIN            6   // PWM ou données pour NeoPixel
    #define LED_PWM_PIN             11  // PWM alternative
    
    // Analog sensors
    #define BATTERY_SENSE_PIN       A0  // Tension batterie
    #define TEMPERATURE_PIN         A1  // Température (optionnel)
    
    // Autres
    #define BUTTON_PIN              2   // Bouton mode
    #define STATUS_LED_PIN          13  // LED interne

// ============================================================================
// PIN DEFINITIONS - ARDUINO MEGA
// ============================================================================

#elif defined(PLATFORM_ARDUINO_MEGA)
    #define GPS_RX_PIN              19  // RX1
    #define GPS_TX_PIN              18  // TX1
    #define GPS_BAUD_RATE           9600
    
    #define I2C_SDA_PIN             20
    #define I2C_SCL_PIN             21
    #define I2C_SPEED               400000
    
    #define LED_DATA_PIN            44
    #define LED_PWM_PIN             45
    
    #define BATTERY_SENSE_PIN       A0
    #define TEMPERATURE_PIN         A1
    
    #define BUTTON_PIN              2
    #define STATUS_LED_PIN          13

// ============================================================================
// PIN DEFINITIONS - ESP32 NANO
// ============================================================================

#elif defined(PLATFORM_ESP32_NANO)
    // GPS Module (UART1: Hardware Serial)
    #define GPS_RX_PIN              3   // RX0 (alternative pin)
    #define GPS_TX_PIN              1   // TX0 (alternative pin)
    #define GPS_UART_NUM            1   // Serial1 for ESP32
    #define GPS_BAUD_RATE           9600
    
    // I2C - Accéléromètre MPU-6050 + Wattmètre INA219
    #define I2C_SDA_PIN             8   // GPIO8 (SDA/D5 on Nano ESP32)
    #define I2C_SCL_PIN             9   // GPIO9 (SCL/D6 on Nano ESP32)
    #define I2C_SPEED               400000  // 400 kHz standard
    
    // MPU-6050 I2C Address
    #define MPU6050_I2C_ADDR        0x68    // Default address
    
    // INA219 I2C Address (wattmètre)
    #define INA219_I2C_ADDR         0x40    // Default address (A0=A1=GND)
    
    // LED Control (GPIO11 = D7 on Nano ESP32)
    #define LED_DATA_PIN            11  // D7 - WS2812B data line
    #define LED_PWM_PIN             10  // D8 - PWM alternative
    
    // Analog sensors
    #define BATTERY_SENSE_PIN       4   // GPIO4 (A0 analog)
    #define TEMPERATURE_PIN         5   // GPIO5 (A1 analog)
    
    // Button & Status
    #define BUTTON_PIN              7   // GPIO7 (D4)
    #define STATUS_LED_PIN          6   // GPIO6 (D3)

#endif // Platform selection

// ============================================================================
// GPS CONFIGURATION
// ============================================================================

#define GPS_MODULE_TYPE         GPS_MODULE_BEITIAN_220T
#define GPS_TIMEOUT_MS          2000
#define GPS_READ_INTERVAL_MS    1000

// Beitian 220T specifics
#define GPS_BAUD_RATE_BEITIAN   9600
#define GPS_SENTENCE_BUFFER_SIZE 256

// ============================================================================
// ACCELEROMETER CONFIGURATION (MPU-6050)
// ============================================================================

#define ACCELEROMETER_I2C_ADDR  0x68    // MPU-6050 default address
#define ACCELEROMETER_SCALE     ACCEL_SCALE_2G
#define ACCELEROMETER_SAMPLE_RATE 50    // Hz
#define ACCELEROMETER_FILTER    KALMAN  // ou EXPONENTIAL
#define MPU6050_DLPF_CFG        6       // Low Pass Filter config (0-6)

// ============================================================================
// WATTMETER CONFIGURATION (INA219)
// ============================================================================

#define POWER_MONITOR_ENABLED   1
#define INA219_I2C_ADDR         0x40    // Default address
#define INA219_SHUNT_OHMS       0.1f    // 100 mOhm shunt resistor
#define INA219_MAX_AMPS         3.2f    // Max current: 3.2A
#define INA219_UPDATE_INTERVAL  1000    // ms

// ============================================================================
// LED CONFIGURATION
// ============================================================================

#define LED_TYPE                LED_TYPE_WS2812B
#define LED_NUM_PIXELS          30
#define LED_UPDATE_FREQ         60      // Hz
#define LED_BRIGHTNESS_DEFAULT  200     // 0-255

// Color presets (RGB)
#define LED_COLOR_RED           255, 0, 0
#define LED_COLOR_GREEN         0, 255, 0
#define LED_COLOR_BLUE          0, 0, 255
#define LED_COLOR_YELLOW        255, 255, 0
#define LED_COLOR_WHITE         255, 255, 255
#define LED_COLOR_OFF           0, 0, 0

// ============================================================================
// CLOUD / WIFI CONFIGURATION  
// ============================================================================

#define CLOUD_WIFI_SSID         "YOUR_SSID"
#define CLOUD_WIFI_PASSWORD     "YOUR_PASSWORD"

#define CLOUD_SERVER            "api.example.com"
#define CLOUD_PORT              8080
#define CLOUD_SYNC_INTERVAL     10000   // ms
#define CLOUD_TIMEOUT_MS        5000

// MQTT (alternatif)
#define CLOUD_USE_MQTT          0
#define CLOUD_MQTT_BROKER       "mqtt.example.com"
#define CLOUD_MQTT_PORT         1883
#define CLOUD_MQTT_USER         ""
#define CLOUD_MQTT_PASS         ""

// ============================================================================
// BATTERY & POWER
// ============================================================================

#define BATTERY_MAX_VOLTAGE     4.2f    // V (LiPo)
#define BATTERY_MIN_VOLTAGE     3.0f    // V (cutoff)
#define BATTERY_ADC_MAX         1023    // 10-bit ADC
#define BATTERY_VOLTAGE_DIVIDER 2.0f    // Ratio du voltage divider

// ============================================================================
// GENERAL CONFIGURATION
// ============================================================================

#define DEBUG_MODE              1       // 1 pour verbose logging
#define DEBUG_BAUD_RATE         115200

#define SYSTEM_NAME             "BIXXY"
#define SYSTEM_VERSION          "1.0.0"

// ============================================================================
// TIMING & PERFORMANCE
// ============================================================================

#define MAIN_LOOP_TIMEOUT_MS    100     // Protection watchdog
#define I2C_TIMEOUT_MS          1000
#define UART_TIMEOUT_MS         500
#define NETWORK_TIMEOUT_MS      5000

// ============================================================================
// MEMORY OPTIMIZATION
// ============================================================================

#define USE_SMALL_BUFFER        1       // Si RAM limité (Uno)
#define MAX_PAYLOAD_SIZE        256
#define MAX_URL_LENGTH          128

// ============================================================================
// FEATURE FLAGS
// ============================================================================

#define FEATURE_GPS             1
#define FEATURE_ACCELEROMETER   1
#define FEATURE_LEDS            1
#define FEATURE_CLOUD           1
#define FEATURE_LOGGING         1
#define FEATURE_BATTERY_MONITOR 1

#endif // CONFIG_H
