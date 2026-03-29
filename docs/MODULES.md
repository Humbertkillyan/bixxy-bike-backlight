# Documentation des Modules

## 📦 Vue d'ensemble Modulaire

Chaque module BIXXY est conçu pour être:
- **Indépendant** - Peut être utilisé isolément
- **Réutilisable** - Code découpé et bien séparé
- **Testable** - Interface claire et prévisible
- **Documenté** - Code commenté et docs complètes

---

## 🌍 Module GPS (`src/gps/`)

### Interface

```c
#ifndef GPS_H
#define GPS_H

typedef struct {
    double latitude;
    double longitude;
    float altitude;
    float speed;
    float course;
    uint8_t satellites;
    uint32_t timestamp;
} gps_data_t;

bool gps_init(void);
bool gps_read(gps_data_t *data);
bool gps_is_ready(void);
void gps_deinit(void);
```

### Utilisation

```c
#include "src/gps/gps.h"

void setup() {
    gps_init();
}

void loop() {
    gps_data_t data;
    
    if (gps_read(&data)) {
        Serial.print("Lat: ");
        Serial.println(data.latitude, 6);
        Serial.print("Lon: ");
        Serial.println(data.longitude, 6);
    }
    
    delay(1000);
}
```

### Spécifications Techniques

| Paramètre | Valeur |
|-----------|--------|
| Protocole | UART (9600 baud) |
| Format | NMEA-0183 |
| Fréquence lecture | 1 Hz |
| Précision | 2.5m RMS |
| TTFF (à froid) | <30s |

### Sentences Soutenues

- `$GPRMC` - Recommended Minimum
- `$GPGGA` - Fix Data
- `$GPGSA` - DOP and Active Satellites
- `$GPGSV` - Satellites in View

### Limitations et Notes

- Nécessite vue sur le ciel (outdoor)
- Warm-up: ~5-10 secondes
- Données invalides tant que pas lock
- Dérive possible en environnement urbain

---

## ⚡ Module Accéléromètre (`src/accelerometer/`)

### Interface

```c
#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

typedef struct {
    int16_t x, y, z;        // Raw values
    float ax, ay, az;       // Converted to g (±G)
    float magnitude;        // Intensité
} accel_data_t;

typedef enum {
    ACCEL_SCALE_2G,
    ACCEL_SCALE_4G,
    ACCEL_SCALE_8G,
    ACCEL_SCALE_16G
} accel_scale_t;

bool accelerometer_init(void);
bool accelerometer_configure(accel_scale_t scale);
bool accelerometer_read(accel_data_t *data);
bool accelerometer_calibrate(void);
void accelerometer_deinit(void);
```

### Utilisation

```c
#include "src/accelerometer/accelerometer.h"

void setup() {
    accelerometer_init();
    accelerometer_configure(ACCEL_SCALE_2G);
    accelerometer_calibrate();  // Important!
}

void loop() {
    accel_data_t data;
    
    if (accelerometer_read(&data)) {
        Serial.print("X: ");
        Serial.print(data.ax);
        Serial.print(" g, Magnitude: ");
        Serial.println(data.magnitude);
        
        // Détection de mouvement
        if (data.magnitude > 1.5f) {
            // Mouvement détecté
        }
    }
    
    delay(20);  // 50 Hz
}
```

### Spécifications Techniques

| Paramètre | Valeur |
|-----------|--------|
| Capteur | MPU-6050 / 9250 |
| Interface | I2C @ 400kHz |
| Résolution | 16-bit |
| Range | ±2g à ±16g (configurable) |
| Fréquence | 50-200 Hz (configurable) |

### Axes d'Accélération

```
        Z
        ↑
        │
X ←─────┼────→  Y
        │
        ↓
```

### Calibration

Minimum une fois au démarrage:
```c
accelerometer_calibrate();  // ~5 secondes
```

---

## 💡 Module LEDs (`src/leds/`)

### Interface

```c
#ifndef LEDS_H
#define LEDS_H

typedef struct {
    uint8_t r, g, b;
} rgb_color_t;

typedef enum {
    LED_MODE_OFF,
    LED_MODE_STATIC,
    LED_MODE_PULSE,
    LED_MODE_RAINBOW,
    LED_MODE_CHASE,
    LED_MODE_ADAPTIVE
} led_mode_t;

bool leds_init(void);
bool leds_set_mode(led_mode_t mode);
bool leds_fill_color(uint8_t r, uint8_t g, uint8_t b);
bool leds_set_brightness(uint8_t brightness);  // 0-255
bool leds_update(void);
void leds_deinit(void);
```

### Utilisation

```c
#include "src/leds/leds.h"

void setup() {
    leds_init();
}

void loop() {
    // Mode statique - rouge
    leds_set_mode(LED_MODE_STATIC);
    leds_fill_color(255, 0, 0);
    leds_update();
    
    delay(2000);
    
    // Mode pulse
    leds_set_mode(LED_MODE_PULSE);
    leds_fill_color(0, 255, 0);
    leds_update();
    
    delay(2000);
    
    // Mode adaptatif (basé sur vitesse)
    leds_set_mode(LED_MODE_ADAPTIVE);
    leds_update();
    
    delay(100);
}
```

### Modes Disponibles

1. **OFF** - Toutes LEDs éteintes
2. **STATIC** - Couleur fixe
3. **PULSE** - Pulsation lente
4. **RAINBOW** - Arc-en-ciel rotatif
5. **CHASE** - Poursuivant LED par LED
6. **ADAPTIVE** - Couleur selon vitesse/état

### Types de LEDs

#### WS2812B (NeoPixel)
- Connecté sur 1 pin GPIO
- 3 bytes par LED (RGB)
- Timing critique (100ns)

#### GPIO Simple
- LED traditionnelle
- Pin on/off ou PWM
- Plus simple, moins d'effet

---

## ☁️ Module Cloud (`src/cloud/`)

### Interface

```c
#ifndef CLOUD_H
#define CLOUD_H

typedef struct {
    uint32_t timestamp;
    double latitude;
    double longitude;
    float speed;
    uint8_t battery;
} cloud_payload_t;

typedef enum {
    CLOUD_STATE_DISCONNECTED,
    CLOUD_STATE_CONNECTING,
    CLOUD_STATE_CONNECTED,
    CLOUD_STATE_ERROR
} cloud_state_t;

bool cloud_init(const char *ssid, const char *password);
cloud_state_t cloud_get_state(void);
bool cloud_send_data(const cloud_payload_t *data);
bool cloud_receive_command(char *buffer, size_t len);
void cloud_deinit(void);
```

### Utilisation

```c
#include "src/cloud/cloud.h"

void setup() {
    cloud_init("MY_SSID", "MY_PASSWORD");
}

void loop() {
    if (cloud_get_state() == CLOUD_STATE_CONNECTED) {
        cloud_payload_t data = {
            .timestamp = millis(),
            .latitude = gps_data.latitude,
            .longitude = gps_data.longitude,
            .speed = gps_data.speed,
            .battery = 85
        };
        
        if (cloud_send_data(&data)) {
            Serial.println("Data sent!");
        }
    }
    
    delay(10000);  // Upload tous les 10s
}
```

### Format de Données

#### JSON Envoyé
```json
{
    "ts": 1234567890,
    "lat": 48.8566,
    "lon": 2.3522,
    "speed": 15.5,
    "battery": 85
}
```

#### Commandes Reçues
```json
{
    "cmd": "led_mode",
    "mode": "rainbow"
}
```

### Endpoints Cloud

```
POST /api/bike/telemetry
GET  /api/bike/commands
POST /api/bike/heartbeat
```

---

## 📊 Module Capteurs (`src/sensors/`)

### Interface

```c
#ifndef SENSORS_H
#define SENSORS_H

typedef struct {
    gps_data_t gps;
    accel_data_t accel;
    float battery_voltage;
    float temperature;
    uint32_t uptime;
} sensor_fusion_t;

bool sensors_init(void);
bool sensors_update_all(sensor_fusion_t *fused);
bool sensors_get_speed(float *speed);
bool sensors_is_moving(void);
void sensors_deinit(void);
```

### Fusion de Données

Combine les données de multiples capteurs:
- GPS pour vitesse et position
- Accéléromètre pour détection de mouvement
- Batterie et température
- Timestamps et uptime

### Utilisation

```c
#include "src/sensors/sensors.h"

sensor_fusion_t fused_data;
sensors_update_all(&fused_data);

if (sensors_is_moving()) {
    // Vélo en mouvement
    leds_set_mode(LED_MODE_ADAPTIVE);
} else {
    // Vélo stationnaire
    leds_set_mode(LED_MODE_OFF);
}
```

---

## 🛠️ Module Utils (`src/utils/`)

### Fonctions Disponibles

```c
// Math utilities
float utils_map(float x, float in_min, float in_max, 
                float out_min, float out_max);
float utils_constrain(float x, float min, float max);
float utils_distance_haversine(double lat1, double lon1, 
                               double lat2, double lon2);

// String utilities
void utils_itoa(int value, char *str, int base);
void utils_float_to_str(float value, char *str, int decimals);

// Timing
uint32_t utils_millis(void);
void utils_delay(uint32_t ms);

// CRC and validation
uint8_t utils_crc8(const uint8_t *data, size_t len);
bool utils_validate_nmea(const char *sentence);

// Logging
void utils_log(const char *level, const char *msg);
```

---

## 🔄 Dépendances Entre Modules

```graph
    main.c (orchestration)
        ├── gps.c
        ├── accelerometer.c
        ├── leds.c
        ├── sensors.c (agrège gps + accel)
        └── cloud.c (envoie données fusionnées)
```

---

## 📈 Performances

| Module | RAM | Flash | CPU Usag |
|--------|-----|-------|----------|
| GPS | 256B | 2KB | 5% @1Hz |
| Accéléromètre | 128B | 1.5KB | 10% @50Hz |
| LEDs | 512B | 3KB | 20% @60Hz |
| Cloud | 2KB | 4KB | 15% @0.1Hz |
| Sensors | 512B | 1KB | 1% |
| Utils | 128B | 2KB | Var |

---

*Documentation complète - Mise à jour Mars 2026*
