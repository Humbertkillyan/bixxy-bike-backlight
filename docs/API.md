# Référence API Complète

## 📖 Vue d'ensemble

Cette page documente toutes les fonctions publiques disponibles dans BIXXY.

---

## 🌍 GPS API

### Types

```c
typedef struct {
    double latitude;        // °N, -90..90
    double longitude;       // °E, -180..180
    float altitude;        // mètres
    float speed;           // km/h
    float course;          // degrés (0-359)
    uint8_t satellites;    // nombre de satellites
    uint32_t timestamp;    // ms depuis démarrage
} gps_data_t;
```

### Fonctions

#### `gps_init()`
```c
bool gps_init(void);
```
- **Paramètres**: Aucun
- **Retour**: `true` si succès, `false` si erreur
- **Description**: Initialise le port série et le module GPS
- **Remarques**: À appeler une seule fois au démarrage

#### `gps_read(gps_data_t *data)`
```c
bool gps_read(gps_data_t *data);
```
- **Paramètres**: 
  - `data` - Pointeur vers structure à remplir
- **Retour**: `true` si nouvelle donnée, `false` si pas de changement
- **Description**: Lit les données GPS disponibles
- **Fréquence max**: 1 Hz (limitée par le module)

#### `gps_is_ready()`
```c
bool gps_is_ready(void);
```
- **Retour**: `true` si GPS a lock, `false` sinon
- **Description**: Vérifie si le GPS a obtenu un fix

#### `gps_deinit()`
```c
void gps_deinit(void);
```
- **Description**: Arrête le module GPS

---

## ⚡ Accelerometer API

### Types

```c
typedef struct {
    int16_t x, y, z;       // Raw values (-32768..32767)
    float ax, ay, az;      // Convertis en g
    float magnitude;       // √(ax² + ay² + az²)
} accel_data_t;

typedef enum {
    ACCEL_SCALE_2G,        // ±2g
    ACCEL_SCALE_4G,        // ±4g
    ACCEL_SCALE_8G,        // ±8g
    ACCEL_SCALE_16G        // ±16g
} accel_scale_t;
```

### Fonctions

#### `accelerometer_init()`
```c
bool accelerometer_init(void);
```
- **Retour**: `true` si I2C OK
- **Description**: Lance la communication I2C et configure l'accéléromètre

#### `accelerometer_configure(accel_scale_t scale)`
```c
bool accelerometer_configure(accel_scale_t scale);
```
- **Paramètres**:
  - `scale` - Plage de mesure (2g, 4g, 8g ou 16g)
- **Retour**: `true` si configuré
- **Remarques**: 2g = plus de précision, 16g = plus de range

#### `accelerometer_read(accel_data_t *data)`
```c
bool accelerometer_read(accel_data_t *data);
```
- **Paramètres**:
  - `data` - Structure à remplir
- **Retour**: `true` si lecture OK
- **Fréquence max**: 200 Hz

#### `accelerometer_calibrate()`
```c
bool accelerometer_calibrate(void);
```
- **Retour**: `true` si OK, `false` si timeout
- **Durée**: ~5 secondes
- **Remarques**: À appeler au démarrage, sur surface plane

#### `accelerometer_deinit()`
```c
void accelerometer_deinit(void);
```
- **Description**: Arrête l'accéléromètre

---

## 💡 LEDs API

### Types

```c
typedef struct {
    uint8_t r, g, b;       // 0-255 chaque
} rgb_color_t;

typedef enum {
    LED_MODE_OFF,          // Éteint
    LED_MODE_STATIC,       // Couleur fixe
    LED_MODE_PULSE,        // Clignotement lent
    LED_MODE_RAINBOW,      // Arc-en-ciel
    LED_MODE_CHASE,        // Poursuite
    LED_MODE_ADAPTIVE      // Selon vitesse
} led_mode_t;
```

### Fonctions

#### `leds_init()`
```c
bool leds_init(void);
```
- **Retour**: `true` si OK
- **Description**: Initialise les LEDs et configure les pins

#### `leds_set_mode(led_mode_t mode)`
```c
bool leds_set_mode(led_mode_t mode);
```
- **Paramètres**:
  - `mode` - Mode d'affichage
- **Retour**: `true` si configuré
- **Remarques**: Le changement de mode prend effet au prochain `update`

#### `leds_fill_color(uint8_t r, uint8_t g, uint8_t b)`
```c
bool leds_fill_color(uint8_t r, uint8_t g, uint8_t b);
```
- **Paramètres**:
  - `r`, `g`, `b` - Valeurs RGB (0-255)
- **Retour**: `true` si OK
- **Remarques**: Affecte seulement mode STATIC

#### `leds_set_brightness(uint8_t brightness)`
```c
bool leds_set_brightness(uint8_t brightness);
```
- **Paramètres**:
  - `brightness` - 0-255 (0=off, 255=max)
- **Retour**: `true` if OK
- **Remarques**: Affecte tous les modes

#### `leds_update()`
```c
bool leds_update(void);
```
- **Retour**: `true` si OK
- **Description**: Applique les changements (envoie à LEDs)
- **Remarques**: À appeler régulièrement (60 Hz recommandé)

#### `leds_deinit()`
```c
void leds_deinit(void);
```
- **Description**: Arrête les LEDs

---

## ☁️ Cloud API

### Types

```c
typedef struct {
    uint32_t timestamp;
    double latitude;
    double longitude;
    float speed;
    uint8_t battery;
    char location[32];
} cloud_payload_t;

typedef enum {
    CLOUD_STATE_DISCONNECTED = 0,
    CLOUD_STATE_CONNECTING = 1,
    CLOUD_STATE_CONNECTED = 2,
    CLOUD_STATE_ERROR = 3
} cloud_state_t;
```

### Fonctions

#### `cloud_init(const char *ssid, const char *password)`
```c
bool cloud_init(const char *ssid, const char *password);
```
- **Paramètres**:
  - `ssid` - SSID WiFi
  - `password` - Mot de passe WiFi
- **Retour**: `true` si OK
- **Durée**: ~5-10 secondes pour connexion

#### `cloud_get_state()`
```c
cloud_state_t cloud_get_state(void);
```
- **Retour**: État actuel
- **Remarques**: Non-bloquant

#### `cloud_send_data(const cloud_payload_t *data)`
```c
bool cloud_send_data(const cloud_payload_t *data);
```
- **Paramètres**:
  - `data` - Données à envoyer
- **Retour**: `true` si succès
- **Remarques**: Non-bloquant (retourne immédiatement)

#### `cloud_receive_command(char *buffer, size_t len)`
```c
bool cloud_receive_command(char *buffer, size_t len);
```
- **Paramètres**:
  - `buffer` - Buffer pour la commande
  - `len` - Taille max
- **Retour**: `true` si commande reçue
- **Remarques**: Non-bloquant

#### `cloud_deinit()`
```c
void cloud_deinit(void);
```
- **Description**: Ferme la connexion WiFi

---

## 📊 Sensors API

### Types

```c
typedef struct {
    gps_data_t gps;
    accel_data_t accel;
    float battery_voltage;
    float temperature;
    uint32_t uptime;
    bool is_moving;
    float ground_speed;
} sensor_fusion_t;
```

### Fonctions

#### `sensors_init()`
```c
bool sensors_init(void);
```
- **Retour**: `true` si tous initialisés
- **Description**: Initialise tous les capteurs

#### `sensors_update_all(sensor_fusion_t *fused)`
```c
bool sensors_update_all(sensor_fusion_t *fused);
```
- **Paramètres**:
  - `fused` - Structure à remplir
- **Retour**: `true` si mis à jour
- **Fréquence**: Appeler à ~10 Hz

#### `sensors_get_speed(float *speed)`
```c
bool sensors_get_speed(float *speed);
```
- **Paramètres**:
  - `speed` - Vitesse en km/h
- **Retour**: `true` si disponible

#### `sensors_is_moving()`
```c
bool sensors_is_moving(void);
```
- **Retour**: `true` si accélération détectée

#### `sensors_deinit()`
```c
void sensors_deinit(void);
```
- **Description**: Arrête tous les capteurs

---

## 🛠️ Utils API

### Math Functions

#### `utils_map()`
```c
float utils_map(float x, float in_min, float in_max, 
                float out_min, float out_max);
```
- **Description**: Mappe une valeur d'un range à un autre

#### `utils_constrain()`
```c
float utils_constrain(float x, float min, float max);
```
- **Description**: Limite une valeur dans un range

#### `utils_distance_haversine()`
```c
float utils_distance_haversine(double lat1, double lon1,
                               double lat2, double lon2);
```
- **Retour**: Distance en mètres

### String Functions

#### `utils_float_to_str()`
```c
void utils_float_to_str(float value, char *str, int decimals);
```
- **Description**: Convertit un float en string

### Logging

#### `utils_log()`
```c
void utils_log(const char *level, const char *msg);
```
- **Paramètres**:
  - `level` - "ERROR", "WARN", "INFO", "DEBUG"
  - `msg` - Message

---

## 🔢 Status Codes

```c
#define GPS_OK                0
#define GPS_NO_FIX           -1
#define GPS_TIMEOUT          -2
#define GPS_UART_ERROR       -3

#define ACCEL_OK              0
#define ACCEL_I2C_ERROR      -1
#define ACCEL_NOT_READY      -2

#define LED_OK                0
#define LED_INVALID_MODE     -1
#define LED_TIMEOUT          -2

#define CLOUD_OK              0
#define CLOUD_NO_WIFI        -1
#define CLOUD_SEND_ERROR     -2
#define CLOUD_TIMEOUT        -3
```

---

*Référence API v1.0 - Mars 2026*
