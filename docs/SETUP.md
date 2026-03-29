# Configuration et Installation

## 📋 Prérequis

### Matériel
- **Microcontrôleur**: Arduino Uno, Mega, ou ESP32
- **GPS Module**: Neo-6M ou compatible UBLOX (UART-based)
- **Accéléromètre**: MPU-6050, MPU-9250 ou compatible (I2C/SPI)
- **LEDs**: WS2812 (NeoPixel) ou GPIO simples
- **Connectivité**: Module WiFi (ESP32 intégré ou externe)
- **Alimentation**: Batterie LiPo ou USB

### Logiciels
- **Option 1**: [Arduino IDE](https://www.arduino.cc/en/software) (v1.8.13+)
- **Option 2**: [PlatformIO](https://platformio.org) (recommandé)
- **Git**: [Git for Windows](https://git-scm.com)

## 🚀 Installation Rapide (PlatformIO)

### 1. Installer PlatformIO

```bash
# Installer l'extension VS Code
# Ou si vous préférez l'CLI:
pip install -U platformio
```

### 2. Cloner le Repo

```bash
git clone https://github.com/YOUR_USERNAME/bixxy-bike-backlight.git
cd bixxy-bike-backlight
```

### 3. Configuration Hardware

Éditer `src/config.h`:

```c
// Sélectionner votre plateforme
#define PLATFORM_ARDUINO_UNO  1
// #define PLATFORM_ESP32      0

// Pin definitions pour votre setup
#define GPS_RX_PIN    0
#define GPS_TX_PIN    1
#define I2C_SDA_PIN   A4  // Arduino Uno
#define I2C_SCL_PIN   A5  // Arduino Uno
#define LED_PIN       6
```

### 4. Compiler et Upload

```bash
# Compiler
platformio run -e arduino_uno

# Compiler et upload sur le port COM3 (par ex)
platformio run --target upload -e arduino_uno

# Surveiller la sortie série
platformio device monitor -b 115200
```

## 🔧 Configuration Détaillée

### GPS Module Configuration

Éditer `src/gps/gps.h`:

```c
#define GPS_BAUD_RATE 9600      // Standard ublox
#define GPS_TIMEOUT_MS 2000
#define GPS_READ_INTERVAL_MS 1000

typedef enum {
    GPS_MODULE_UBLOX,
    GPS_MODULE_MTKCV5,
    GPS_MODULE_SIM68M
} gps_module_type_t;

#define GPS_TYPE GPS_MODULE_UBLOX
```

### Accéléromètre Configuration

```c
// src/accelerometer/accelerometer.h
#define ACCEL_I2C_ADDRESS 0x68  // MPU-6050
#define ACCEL_SCALE 2           // ±2g
#define ACCEL_SAMPLING_RATE 50  // Hz
```

### LEDs Configuration

```c
// src/leds/leds.h
#define LED_TYPE_WS2812B  1     // NeoPixel
// #define LED_TYPE_GPIO   0

#define LED_NUM_PIXELS    30    // Nombre de LEDs
#define LED_DATA_PIN      6
#define LED_UPDATE_FREQ   60    // Hz
```

### Cloud Configuration

```c
// src/cloud/cloud.h
#define CLOUD_WIFI_SSID      "YOUR_SSID"
#define CLOUD_WIFI_PASSWORD  "YOUR_PASSWORD"
#define CLOUD_SERVER         "api.example.com"
#define CLOUD_PORT           8080
#define CLOUD_SYNC_INTERVAL  10000  // ms
```

## 🔌 Wiring (Schéma de câblage)

### Arduino Uno + GPS + MPU-6050 + WS2812B

```
GPS Module (Neo-6M):
  VCC -> 5V
  GND -> GND
  TX  -> RX (pin 0)
  RX  -> TX (pin 1) through voltage divider

MPU-6050:
  VCC -> 5V
  GND -> GND
  SCL -> A5 (SCL)
  SDA -> A4 (SDA)

WS2812B LED Strip:
  5V  -> 5V (through logic shifter or direct)
  GND -> GND
  DIN -> Pin 6 (with 470Ω resistor)
```

### ESP32 Configuration

```
GPS Module:
  TX -> GPIO16 (RX2)
  RX -> GPIO17 (TX2)

MPU-6050:
  SCL -> GPIO22
  SDA -> GPIO21

WS2812B:
  DIN -> GPIO5
```

## 🧪 Tests de Vérification

### 1. Test des Ports Série

```bash
# Vérifier la connexion GPS
platformio device monitor -b 9600 -p COM3
```

Devez voir des données NMEA comme:
```
$GPRMC,123519,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
```

### 2. Test I2C Scanner

Upload l'exemple Arduino I2C Scanner:
```cpp
#include <Wire.h>

void setup() {
    Serial.begin(115200);
    Wire.begin();
}

void loop() {
    for (byte i = 8; i < 120; i++) {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0) {
            Serial.print("I2C device found at 0x");
            Serial.println(i, HEX);
        }
    }
    delay(5000);
}
```

Attendu: `I2C device found at 0x68` (MPU-6050)

### 3. Test LED

```cpp
#include "src/leds/leds.h"

void setup() {
    leds_init();
}

void loop() {
    leds_fill_color(255, 0, 0);  // Rouge
    delay(1000);
    leds_fill_color(0, 255, 0);  // Vert
    delay(1000);
}
```

## ⚠️ Troubleshooting

### GPS n'envoie pas de données
- Vérifier réception < 3 secondes (froide)
- Assurer outdoor ou fenêtre
- Vérifier tension (4.5-5.5V)
- Tester avec u-center (logiciel UBLOX)

### MPU-6050 non détecté sur I2C
- Vérifier les pull-ups (4.7k récommandé)
- Vérifier l'adresse (0x68 ou 0x69)
- Tester avec I2C Scanner
- Vérifier tensions (3.3V ou 5V avec convertisseur)

### LEDs ne s'allument pas
- Vérifier l'ordre RGB vs BGR
- Vérifier la tension 5V suffisante
- Tester avec une seule LED
- Résolveur les problèmes de timing

### Upload échoue
```bash
# Réinitialiser le board
platformio device reset

# Essayer manuelle:
# Appuyer sur RESET avant upload
# PlatformIO devrait compiler et upload automatiquement
```

## 📚 Ressources Additionnelles

- [Arduino Official Docs](https://docs.arduino.cc)
- [PlatformIO Documentation](https://docs.platformio.org)
- [MPU-6050 Datasheet](https://www.invensense.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
- [UBLOX Neo-6M Docs](https://www.u-blox.com/en/product/neoptech-2016-gps)

---
*Dernière mise à jour: Mars 2026*
