# BIXXY Hardware Setup Guide

**Projet:** BIXXY Bike Backlight  
**Plateforme:** Arduino Nano ESP32  
**Date:** March 2026

---

## 📋 Composants utilisés

### Microcontrôleur
- **Arduino Nano ESP32** (S3)
  - Processeur: Dual-core Xtensa 32-bit LX7 @ 240 MHz
  - RAM: 8 MB PSRAM
  - Flash: 16 MB
  - Connectivité: WiFi 802.11 b/g/n, BLE 5.0
  - Tensions: 3.3V (toutes entrées/sorties)
  - Courant: 200 mA nominal

### Capteurs

#### 1. Accéléromètre: MPU-6050
- **Spécifications:**
  - 3 axes accélération
  - Plage: ±2g, ±4g, ±8g, ±16g (sélectionnable)
  - Fréquence: jusqu'à 200 Hz
  - Interface: I2C (400 kHz)
  - Adresse I2C: 0x68 (par défaut)
  - Tension d'alimentation: 3.0V - 3.5V (3.3V OK)
  - Courant: ~3.8 mA

- **Brochage (GY-521 breakout board):**
  ```
  VCC  → 3.3V
  GND  → GND
  SCL  → GPIO9  (D6 sur Nano ESP32)  [I2C Clock]
  SDA  → GPIO8  (D5 sur Nano ESP32)  [I2C Data]
  INT  → GPIO2  (optionnel: interruption)
  ```

- **Bibliothèque:** `electroniccats/MPU6050` v0.1.0
  - GitHub: https://github.com/electroniccats/mpu6050
  - Fournit classe `MPU6050` avec interface complète

#### 2. GPS: Beitian 220T
- **Spécifications:**
  - Chipset: u-blox LEA-6T
  - Fréquence: 1575.42 MHz (L1)
  - Baudrate: 9600 baud (configurable)
  - Sortie: Sentences NMEA (GGA, RMC, GSA, etc.)
  - Précision: ±2.5m (horizontal)
  - Acquisizione: <1s (hot start)
  - Alimentation: 3.3V - 5.5V (3.3V recommandé pour le nano)
  - Courant: 30 mA nominal

- **Brochage (série 6 broches):**
  ```
  VCC  → 3.3V
  GND  → GND
  RX   → GPIO3  (D9 alt)
  TX   → GPIO1  (D10 alt)
  PPS  → NC     (Time Pulse - optionnel)
  GND  → GND
  ```

- **Configuration UART:**
  - Port: Serial1 (UART1)
  - Baud: 9600
  - Format: 8N1 (8 données, 1 stop, pas de parité)

- **Bibliothèque:** `TinyGPSPlus`
  - Parsing NMEA complet
  - Extraction: latitude, longitude, altitude, vitesse, cap, satellites

- **Ressources Beitian:**
  - Documents: https://store.beitian.com/blogs/news/gps-modules-with-arduino-and-raspberry-pi

#### 3. Wattmètre: INA219
- **Spécifications:**
  - Mesure de courant/puissance
  - Tension bus: 0V à 26V (ou 32V modèles)
  - Courant: jusqu'à 3.2A (avec shunt 0.1Ω)
  - Interface: I2C (400 kHz max)
  - Adresse I2C: 0x40 (par défaut, configurable via A0/A1)
  - Résistance shunt: 0.1Ω (typique pour ±3.2A)
  - Tension d'alimentation: 3.0V - 5.5V (3.3V OK)
  - Courant: ~0.5 mA

- **Mesures fournies:**
  - Bus Voltage: Tension d'alimentation
  - Shunt Voltage: Chute tension sur résisteur shunt
  - Current: Courant calculé (V_shunt / R_shunt)
  - Power: Puissance (V_bus × I_shunt)

- **Brochage (SCT013-030 breakout):**
  ```
  VCC  → 3.3V
  GND  → GND
  SCL  → GPIO9  (D6)  [I2C Clock - partagé avec MPU6050]
  SDA  → GPIO8  (D5)  [I2C Data - partagé avec MPU6050]
  ```

- **Bibliothèque:** `DFRobot/DFRobot_INA219` v1.0.1
  - GitHub: https://github.com/DFRobot/DFRobot_INA219
  - Classe `DFRobot_INA219` avec calibration automatique

---

## 🔌 Schéma de Câblage - I2C Bus (Partagé)

```
Arduino Nano ESP32
│
├─ GPIO8  (D5/SDA) ── 4.7kΩ pull-up ──┐
│                                       ├─ I2C SDA (3.3V)
├─ GPIO9  (D6/SCL) ── 4.7kΩ pull-up ──┤
│                                       │
├─ 3.3V ─────────────────────────────┬─┤
│                                    │ │
│  ┌─ MPU-6050 ────┐  ┌─ INA219 ──┐ │ │
│  │ VCC ──────────┴──┤ VCC       │ │ │
│  │ GND ──────────┬──┤ GND       │ │ │
│  │ SDA ──────────┴──┤ SDA       │ │ │
│  │ SCL ──────────┬──┤ SCL       │ │ │
│  └───────────────┘  └───────────┘ │ │
│                                    │ │
└────────────────────────────────────┴─┘
(Adresses: 0x68 pour MPU / 0x40 pour INA219)
```

---

## 🔌 Schéma de Câblage - GPS UART

```
Arduino Nano ESP32        Beitian 220T
│
├─ GPIO3  (D9 RX) ────────────→ TX
├─ GPIO1  (D10 TX) ───────────→ RX
├─ 3.3V ──────────────────────→ VCC
└─ GND ───────────────────────→ GND
```

**Note:** Pour la transmission série 5V-safe, considérer un logic level shifter si l'alimentation du GPS doit être 5V.

---

## ⚡ Gestion de l'Alimentation

### Tension recommandée
- Nano ESP32: 5V via USB ou 3.3V direct
- Tous les capteurs: 3.3V max

### Découplage
Ajouter des capacités de découplage près de chaque composant:
- 100nF (0.1µF) en céramique sur VCC (à côté de chaque capteur)
- 4.7kΩ pull-ups sur I2C (recommandé)

### Budget de courant (max)
- Nano ESP32: 200 mA
- MPU-6050: 3.8 mA
- Beitian 220T: 30 mA
- INA219: 0.5 mA
- **Total:** ~235 mA

Avec batterie 18650 (3000 mAh): ~8 heures de fonctionnement continu.

---

## 🛠️ Configuration Software

### Pins dans `config.h`
```c
// ESP32 Nano I2C
#define I2C_SDA_PIN  8      // GPIO8
#define I2C_SCL_PIN  9      // GPIO9
#define I2C_SPEED    400000 // 400 kHz

// MPU-6050
#define MPU6050_I2C_ADDR  0x68

// GPS UART1
#define GPS_RX_PIN   3      // Serial1 RX
#define GPS_TX_PIN   1      // Serial1 TX
#define GPS_BAUD_RATE 9600

// INA219
#define INA219_I2C_ADDR    0x40
#define INA219_SHUNT_OHMS  0.1f
```

### Initialisation dans `main.c`
```c
// Ordre d'initialisation recommandé:
1. power_init()           // INA219 d'abord pour mesurer la tension
2. accelerometer_init()   // MPU-6050 sur I2C
3. gps_init()            // Beitian sur UART1
4. leds_init()           // LEDs suite (GPIO11)
5. cloud_init()          // WiFi (si besoin)
```

---

## 📊 Debugging et Monitoring

### Logs série (Serial @ 115200 baud)
```
[INFO] Initializing MPU-6050 Accelerometer
[INFO] MPU-6050 detected successfully
[INFO] Initializing Beitian 220T GPS module
[INFO] Beitian 220T GPS initialized (9600 baud)
[INFO] Initializing INA219 Power Monitor
[INFO] INA219 detected successfully
```

### Vérification en ligne:
```c
// Tester chaque capteur
bool accel_ok = accelerometer_init();
bool gps_ok = gps_is_ready();
float voltage = power_get_bus_voltage();

if (!accel_ok) {
    Serial.println("ERROR: MPU6050 not responding");
    // Vérifier adresse I2C (0x68), pulls-up, connections
}
```

### Scanner I2C (utilitaire de diagnostic)
Utiliser l'exemple Arduino `Wire > Scanner` pour vérifier:
- 0x68 = MPU-6050
- 0x40 = INA219

---

## 🔧 Troubleshooting

| Symptôme | Cause probable | Solution |
|----------|-----------------|----------|
| MPU6050 not found (0x68) | I2C comm échouée | Vérifier SDA/SCL, pull-ups 4.7kΩ |
| GPS no fix | Module pas alimenté | Vérifier 3.3V, connexions TX/RX |
| INA219 returns 0V | Communication I2C | Vérifier adresse (0x40), connections |
| I2C stuck (SDA low) | Peripheral hanging | Reset ESP32, check for shorts |
| High power consumption | Mode veille non activé | Appeler `power_deinit()` si inutilisé |

---

## 📚 Ressources utiles

- **Beitian 220T Datasheet:** https://store.beitian.com/
- **MPU-6050 Datasheet:** https://invensense.tdk.com/
- **INA219 Datasheet:** https://www.ti.com/product/INA219
- **Arduino Nano ESP32 Pinout:** https://docs.arduino.cc/hardware/nano-esp32

---

## 🚀 Prochaines étapes

- [ ] Intégrer les modules power/gps/accel au main loop
- [ ] Ajouter calibration MPU6050 au démarrage
- [ ] Implémenter détection batterie faible
- [ ] Ajouter WiFi pour cloud sync
- [ ] Logger données vers carte SD (optionnel)

---

**Mis à jour:** 29 March 2026  
**Auteur:** Humbert Killyan  
**Licence:** MIT
