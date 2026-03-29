# BIXXY Bike Backlight 🚴💡

<div align="center">

[![GitHub](https://img.shields.io/badge/GitHub-Humbertkillyan/bixxy--bike--backlight-blue?logo=github)](https://github.com/Humbertkillyan/bixxy-bike-backlight)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino](https://img.shields.io/badge/Arduino-Compatible-brightgreen)](https://www.arduino.cc/)
[![Status: Active Development](https://img.shields.io/badge/Status-Active%20Development-brightgreen)]()

Système de rétroéclairage intelligent pour vélo avec intégration cloud, GPS, accéléromètre et LED intelligentes.

[Documentation](#-documentation) • [Démarrage Rapide](#-démarrage-rapide) • [Architecture](#-architecture) • [Contribution](#-contribution)

</div>

## 📋 Vue d'ensemble

BIXXY est un projet Arduino modulaire conçu pour équiper un vélo de fonctionnalités avancées:
- 🌍 **Suivi GPS** - Localisation en temps réel
- ⚡ **Accéléromètre** - Détection de mouvements et vitesse
- 💡 **Contrôle LED** - Rétroéclairage adaptatif
- ☁️ **Intégration Cloud** - Synchronisation et télémétrie
- 📊 **Capteurs** - Multiples capteurs pour monitoring

## 🏗️ Architecture Modulaire

```
src/
├── main.c              # Point d'entrée principal
├── config.h            # Configuration globale
├── gps/                # Module GPS (NMEA parsing)
├── accelerometer/      # Module accéléromètre (I2C/MPU-6050)
├── leds/               # Module LEDs (WS2812B/GPIO)
├── cloud/              # Module intégration cloud (WiFi/MQTT)
├── sensors/            # Module fusion de capteurs
└── utils/              # Utilitaires partagés
```

Chaque module est **totalement indépendant** avec interface `.h` et implémentation `.c` dédiées.

### Caractéristiques

- 🌍 **Suivi GPS** - Localisation en temps réel via module UBLOX Neo-6M
- ⚡ **Accéléromètre** - Détection mouvements & vitesse via MPU-6050
- 💡 **LEDs Intelligentes** - Rétroéclairage adaptatif WS2812B (NeoPixel)
- ☁️ **Intégration Cloud** - Sync données via WiFi/MQTT
- 📊 **Capteurs** - Fusion multi-capteurs avec timestamps
- 🤖 **IA-Ready** - Documentation pour agents IA (Claude, ChatGPT)

## 🚀 Démarrage Rapide

### Prérequis
- **Arduino Compatibles**: Uno, Mega, ESP32
- **PlatformIO** ou Arduino IDE
- **Hardware**: GPS (Neo-6M), Accéléromètre (MPU-6050), LEDs (WS2812B)

### Installation
```bash
# Cloner le repo
git clone git@github.com:Humbertkillyan/bixxy-bike-backlight.git
cd bixxy-bike-backlight

# Configurer le hardware (pin mappings, WiFi, etc)
nano src/config.h

# Compiler et uploader
platformio run --target upload -e arduino_uno

# Monitorer la sortie série
platformio device monitor -b 115200
```

### Configuration Rapide

Éditer `src/config.h`:
```c
// WiFi
#define CLOUD_WIFI_SSID      "YourNetwork"
#define CLOUD_WIFI_PASSWORD  "YourPassword"

// Cloud server
#define CLOUD_SERVER         "api.example.com"
```

Pour plus de détails → [docs/SETUP.md](docs/SETUP.md)

## 📚 Documentation

| Document | Contenu |
|----------|---------|
| [ARCHITECTURE.md](ARCHITECTURE.md) | Architecture système complète & diagrammes |
| [docs/SETUP.md](docs/SETUP.md) | Installation étape par étape |
| [docs/MODULES.md](docs/MODULES.md) | Documentation technique des 6 modules |
| [docs/API.md](docs/API.md) | Référence API complète avec exemples |
| [docs/DEVELOPMENT.md](docs/DEVELOPMENT.md) | Guide développement & patterns |
| [docs/AI_INSTRUCTIONS.md](docs/AI_INSTRUCTIONS.md) | **Pour agents IA** - Comment contribuer |
| [CONTRIBUTING.md](CONTRIBUTING.md) | Comment contribuer au projet |
| [SUPPORT.md](SUPPORT.md) | Ressources & support |

## 🔧 Modules

### GPS (`src/gps/`)
Gestion du positionnement satellitaire via module GPS/GNSS.

### Accéléromètre (`src/accelerometer/`)
Détection des mouvements et mesure des accélérations.

### LEDs (`src/leds/`)
Contrôle des LEDs avec modes adaptatifs et patterns.

### Cloud (`src/cloud/`)
Communication avec backend cloud pour synchronisation.

### Capteurs (`src/sensors/`)
Gestion générique des capteurs avant/arrière.

### Utils (`src/utils/`)
Fonctions utilitaires partagées.

## 🤖 Intégration avec Agents IA

Ce projet est **spécialement conçu** pour collaboration avec agents IA:

✅ **Strong Documentation** - Patterns, templates, exemples  
✅ **Clear API** - Interfaces simples et prévisibles  
✅ **Modular Architecture** - Facile d'ajouter features  
✅ **AI Guidelines** - Voir [docs/AI_INSTRUCTIONS.md](docs/AI_INSTRUCTIONS.md)

**Exemple**: Demander à Claude:
> "Ajoute un capteur DHT22 en suivant les patterns dans `docs/AI_INSTRUCTIONS.md`. Crée un module indépendant dans `src/temperature/` avec `.h` et `.c`."

Les agents comprendront exactement quoi faire! 🧠

## 📝 Licence

Ce projet est sous licence MIT - voir [LICENSE](LICENSE)

## � Modules Disponibles

### 🌍 GPS Module (`src/gps/`)
Gestion UART + Parsing NMEA pour localisation temps réel via Neo-6M

### ⚡ Accelerometer Module (`src/accelerometer/`)
Communication I2C avec MPU-6050, calibration, conversion accélérations

### 💡 LEDs Module (`src/leds/`)
Support WS2812B (NeoPixel) avec patterns: static, pulse, rainbow, chase, adaptive

### ☁️ Cloud Module (`src/cloud/`)
WiFi + MQTT/HTTP pour sync données vers backend cloud

### 📊 Sensors Module (`src/sensors/`)
Fusion multi-capteurs avec agrégation et timestamps

### 🛠️ Utils Module (`src/utils/`)
Math, logging, validation NMEA, CRC, utilitaires timing

---

## ⭐ Features Futur

- [ ] Support GNSS multi-constellation (GPS + Galileo)
- [ ] Algorithmes de fusion Kalman avancés
- [ ] Mode basse consommation avec sleep
- [ ] Web dashboard pour telemetry
- [ ] OTA (Over-the-air) updates
- [ ] Accelerometer gesture recognition

---

## 🐛 Issues & Bugs

Trouvé un bug? Créez une [issue](https://github.com/Humbertkillyan/bixxy-bike-backlight/issues/new/choose)  
Une question? Ouvrez une [discussion](https://github.com/Humbertkillyan/bixxy-bike-backlight/discussions)

---

## 👥 Contribution

Les contributions sont **très bienvenues**! Voir [CONTRIBUTING.md](CONTRIBUTING.md) pour:
- Comment forker le projet
- Conventions de code
- Workflow PR
- CI/CD checks

---

## 📜 License

MIT License - Voir [LICENSE](LICENSE) pour détails

```
Copyright (c) 2026 BIXXY Project
Permission is hereby granted, free of charge, to any person obtaining a copy...
```

---

<div align="center">

**BIXXY Bike Backlight** - *Faire briller votre vélo intelligemment* ✨

[Discord](https://discord.gg/bixxy) • [Issues](https://github.com/Humbertkillyan/bixxy-bike-backlight/issues) • [Discussions](https://github.com/Humbertkillyan/bixxy-bike-backlight/discussions)

Made with ❤️ by Arduino Cycling Community

</div>
