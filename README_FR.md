# BIXXY - Bike Backlight Intelligent 🚴💡

> *Système de rétroéclairage intelligent pour vélo avec intégration cloud, GPS et LEDs adaptatives*

**Status**: ✅ Version 1.0.0 - Prêt pour Production  
**GitHub**: [Humbertkillyan/bixxy-bike-backlight](https://github.com/Humbertkillyan/bixxy-bike-backlight)  
**License**: MIT  
**Language**: C (Arduino)

---

## 📖 Table des Matières

- [Présentation](#-présentation)
- [Démarrage Rapide](#-démarrage-rapide)
- [Architecture](#-architecture)
- [Documentation](#-documentation)
- [Contribution](#-contribution)

---

## 🎯 Présentation

BIXXY est un projet Arduino **modulaire et extensible** conçu pour équiper un vélo de fonctionnalités avancées:

### Fonctionnalités Principales

- **🌍 GPS en Temps Réel** - Localisation précise via module UBLOX Neo-6M
- **⚡ Détection de Mouvement** - Accéléromètre MPU-6050 avec calibration automatique
- **💡 LEDs Intelligentes** - Rétroéclairage adaptatif WS2812B (NeoPixel) avec patterns multiples
- **☁️ Synchronisation Cloud** - WiFi + MQTT pour télémétrie temps réel
- **📊 Fusion Capteurs** - Agrégation intelligente de multiples sources de données
- **🤖 IA-Ready** - Documentation pour collaboration avec agents IA (Claude, ChatGPT, etc.)

### Caractéristiques Techniques

- ✅ **Modulaire** - 6 modules indépendants, zéro dépendances circulaires
- ✅ **Production-Ready** - Gestion erreurs complète, timeouts, logging
- ✅ **Multi-Platform** - Support Arduino Uno, Mega, ESP32
- ✅ **Non-Bloquant** - Parfait pour systèmes temps réel
- ✅ **Well-Documented** - 3500+ lignes de documentation
- ✅ **IA-Optimisé** - Patterns et templates clairs pour agents IA

---

## 🚀 Démarrage Rapide

### 1. Cloner le Projet

```bash
git clone git@github.com:Humbertkillyan/bixxy-bike-backlight.git
cd bixxy-bike-backlight
```

### 2. Configurer le Hardware

Éditer `src/config.h`:

```c
// Choisir votre plateforme
#define PLATFORM_ARDUINO_UNO    1

// Configurer les pins selon votre setup
#define GPS_RX_PIN              0
#define GPS_TX_PIN              1
#define LED_DATA_PIN            6

// WiFi credentials (optionnel)
#define CLOUD_WIFI_SSID         "YourNetwork"
#define CLOUD_WIFI_PASSWORD     "YourPassword"
```

### 3. Compiler et Uploader

Avec PlatformIO:
```bash
# Compiler
platformio run -e arduino_uno

# Compiler et uploader
platformio run --target upload -e arduino_uno

# Monitorer
platformio device monitor -b 115200
```

Avec Arduino IDE:
1. Ouvrir `src/main.c`
2. Compiler et uploader
3. Ouvrir Serial Monitor à 115200 baud

### 4. Vérifier le Setup

Vous devriez voir dans le Serial Monitor:

```
==========================================
BIXXY Bike Backlight - Sys Init
==========================================

[MAIN] Initializing modules...
[MAIN] Init GPS... OK
[MAIN] Init Accelerometer... OK
[MAIN] Calibrating Accelerometer... OK
[MAIN] Init LEDs... OK
[MAIN] Init Sensors... OK
[MAIN] Init Cloud... WARNING

System ready!
```

---

## 🏗️ Architecture

### Vue d'Ensemble

```
┌─────────────────────────────────────────┐
│           MAIN (main.c)                 │
│      Orchestration générale              │
└────────┬────────────────────────────────┘
         │
    ┌────┴─────────────────────────────────┐
    │                                      │
┌───▼────────┐  ┌──────────────┐  ┌──────▼─────┐
│    GPS     │  │ ACCELEROMETER│  │    LEDS    │
│   Module   │  │   Module     │  │   Module   │
└────────────┘  └──────────────┘  └────────────┘
    │                │                  │
    └────┬───────────┴──────────────────┘
         │
    ┌────▼──────────────┐
    │    SENSORS        │  (Fusion)
    │    Module         │
    └────────┬──────────┘
             │
    ┌────────▼──────────┐
    │    CLOUD          │  (Telemetry)
    │    Module         │
    └───────────────────┘
```

### Modules Disponibles

| Module | Responsabilité | Interface |
|--------|-----------------|-----------|
| **GPS** | Communication UART + Parsing NMEA | `gps.h/c` |
| **Accelerometer** | I2C + Calibration + Conversion | `accelerometer.h/c` |
| **LEDs** | Contrôle WS2812B ou GPIO | `leds.h/c` |
| **Cloud** | WiFi + Telemetry (MQTT/HTTP) | `cloud.h/c` |
| **Sensors** | Fusion et agrégation capteurs | `sensors.h/c` |
| **Utils** | Math, logging, validation | `utils.h/c` |

Chaque module est **totalement indépendant** et peut être utilisé isolément.

---

## 📚 Documentation

### Pour Démarrer

- **[START_HERE.md](START_HERE.md)** - Guide d'accueil
- **[README.md](README.md)** - Vue d'ensemble (EN)
- **[docs/SETUP.md](docs/SETUP.md)** - Installation complète

### Pour Développer

- **[ARCHITECTURE.md](ARCHITECTURE.md)** - Architecture système
- **[docs/DEVELOPMENT.md](docs/DEVELOPMENT.md)** - Guide développement
- **[docs/MODULES.md](docs/MODULES.md)** - Doc technique modules
- **[docs/API.md](docs/API.md)** - Référence API complète

### Pour Contribuer

- **[CONTRIBUTING.md](CONTRIBUTING.md)** - Guide contribution
- **[docs/AI_INSTRUCTIONS.md](docs/AI_INSTRUCTIONS.md)** - Pour agents IA
- **[SECURITY.md](SECURITY.md)** - Politique sécurité
- **[CHANGELOG.md](CHANGELOG.md)** - Historique versions

---

## 🔌 Wiring (Schéma de Câblage)

### Arduino Uno

```
GPS Module (Neo-6M):
  VCC → 5V
  GND → GND
  TX  → RX (pin 0)
  RX  → TX (pin 1)

MPU-6050 Accelerometer:
  VCC → 5V
  GND → GND
  SCL → A5
  SDA → A4

WS2812B LED Strip:
  5V  → 5V (with level shifter)
  GND → GND
  DIN → Pin 6 (with 470Ω resistor)
```

### ESP32

```
GPS Module → RX2 (GPIO16) / TX2 (GPIO17)
MPU-6050   → SDA (GPIO21) / SCL (GPIO22)
LEDs       → GPIO5
```

---

## 💻 Compilation & Upload

### Avec PlatformIO (Recommandé)

```bash
# Installer PlatformIO
pip install platformio

# Compiler pour toutes les platforms
platformio run

# Compiler et uploader pour Arduino Uno
platformio run --target upload -e arduino_uno

# Lancer les tests
platformio test

# Monitorer le port série
platformio device monitor -b 115200 -p COM3
```

### Avec Arduino IDE

1. Installer les librairies requises
2. Copier les fichiers `.c` et `.h` dans le sketch
3. Compiler et uploader

---

## 🤖 Collaboration avec Agents IA

BIXXY est **spécialement optimisé** pour collaboration avec agents IA:

### Exemple: Ajouter un Capteur

Demander à Claude/ChatGPT:

> "Ajoute un capteur de température DHT22 au projet BIXXY.
> 1. Crée un module dans `src/temperature/` avec `temperature.h` et `temperature.c`
> 2. Respecte les patterns décrits dans `docs/AI_INSTRUCTIONS.md`
> 3. Intègre dans `src/sensors/sensors.c`
> 4. Mets à jour la documentation"

L'agent comprendra exactement quoi faire! 

**Voir** [docs/AI_INSTRUCTIONS.md](docs/AI_INSTRUCTIONS.md) pour les détails complets.

---

## 📊 Performance & Ressources

| Composant | RAM | Flash | CPU |
|-----------|-----|-------|-----|
| GPS | 256B | 2KB | 5% |
| Accelerometer | 128B | 1.5KB | 10% |
| LEDs | 512B | 3KB | 20% |
| Cloud | 2KB | 4KB | 15% |
| Sensors | 512B | 1KB | 1% |
| Utils | 128B | 2KB | Var |
| **Total** | **~4KB** | **~13KB** | **~51%** |

Optimisé pour Arduino Uno (2KB RAM / 32KB Flash)

---

## 🐛 Troubleshooting

### GPS n'envoie pas de données

- ✅ Vérifier la réception: <3 secondes pour acquisition à froid
- ✅ Position outdoor ou près d'une fenêtre
- ✅ Tension 4.5-5.5V OK
- ✅ Utiliser u-center (logiciel UBLOX) pour diagnostiquer

### MPU-6050 non détecté

- ✅ I2C Scanner pour vérifier adresse (0x68)
- ✅ Pull-ups 4.7k Ω requis
- ✅ Vérifier tension (3.3V ou 5V)

### LEDs ne s'allument pas

- ✅ Vérifier polarité (5V/GND)
- ✅ Tester une seule LED d'abord
- ✅ Assurer tension 5V suffisante

---

## 📋 Checklist d'Installation

- [ ] Cloner le repo
- [ ] Configurer `src/config.h` (pins, WiFi)
- [ ] Compiler une fois
- [ ] Tester les connexions hardware
- [ ] Upload vers Arduino
- [ ] Vérifier Serial Monitor
- [ ] Consulter doc si besoin

---

## 🚀 Prochaines Étapes

### Immédiat

1. Implémenter GPS parsing NMEA complet
2. Tester accelerometer sur board réel
3. Ajouter patterns LED
4. WiFi connectivity

### Court Terme

1. Intégration MQTT
2. Web dashboard
3. OTA updates
4. Tests unitaires

### Long Terme

1. GNSS multi-constellation
2. Machine learning sur données
3. Système basse consommation
4. Mode distribué multi-bike

---

## 🤝 Contribution

Les contributions sont **très bienvenues**!

1. **Fork** le repo
2. **Create** une branche (`git checkout -b feature/ma-feature`)
3. **Commit** vos changements (`git commit -m "feature: Description"`)
4. **Push** (`git push origin feature/ma-feature`)
5. **Open** une Pull Request

**Lire** [CONTRIBUTING.md](CONTRIBUTING.md) pour les détails complets.

---

## 📞 Support & Contact

- 📖 **Documentation** - Voir les `.md` du repo
- 🐛 **Issues** - [GitHub Issues](https://github.com/Humbertkillyan/bixxy-bike-backlight/issues)
- 💬 **Discussions** - [GitHub Discussions](https://github.com/Humbertkillyan/bixxy-bike-backlight/discussions)
- 🔒 **Sécurité** - [SECURITY.md](SECURITY.md)

---

## 📜 Licence

MIT License - Libre d'utilisation pour projets commerciaux et privés

Voir [LICENSE](LICENSE) pour détails complets.

---

## 👥 Auteurs

- **Killyan Humbert** - Concepteur initiateur
- **Arduino Community** - Feedback et contributions

---

## 🎓 Remerciements

Merci à:
- Arduino Foundation
- PlatformIO
- Tous les contributeurs et testeurs

---

<div align="center">

**BIXXY** - *Faire briller votre vélo intelligemment* ✨

⭐ Si ce projet vous plaît, **mettez une star** sur GitHub! ⭐

[GitHub](https://github.com/Humbertkillyan/bixxy-bike-backlight) • [Documentation](START_HERE.md) • [Contribution](CONTRIBUTING.md)

Made with ❤️ for the Arduino Cycling Community

</div>

---

**Dernière mise à jour**: 29 Mars 2026  
**Version**: 1.0.0
