# BIXXY Bike Backlight 🚴💡

Système de rétroéclairage intelligent pour vélo avec intégration cloud, GPS, accéléromètre et LED intelligentes.

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
├── gps/                # Module GPS
├── accelerometer/      # Module accéléromètre
├── leds/               # Module contrôle LED
├── cloud/              # Module intégration cloud
├── sensors/            # Module capteurs génériques
└── utils/              # Utilitaires
```

Chaque module est indépendant avec `.c` et `.h` dédiés pour une maintenance facile.

## 🚀 Démarrage Rapide

### Prérequis
- Arduino IDE ou PlatformIO
- Matériel Arduino compatible (Uno, Mega, ESP32)
- Librairies requises (voir `platformio.ini`)

### Installation
```bash
git clone https://github.com/YOUR_USERNAME/bixxy-bike-backlight.git
cd bixxy-bike-backlight
```

### Compilation et Upload
```bash
# Avec PlatformIO
platformio run --target upload -e arduino_uno

# Ou avec Arduino IDE
# Ouvrir src/main.c et compiler
```

## 📚 Documentation

- [ARCHITECTURE.md](ARCHITECTURE.md) - Architecture détaillée du système
- [SETUP.md](docs/SETUP.md) - Guide d'installation et configuration
- [MODULES.md](docs/MODULES.md) - Documentation des modules
- [API.md](docs/API.md) - Référence API
- [DEVELOPMENT.md](docs/DEVELOPMENT.md) - Guide de développement

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

Ce projet inclut des instructions détaillées en Markdown pour permettre aux agents IA de:
- Comprendre l'architecture modulaire
- Contribuer au code
- Générer de nouvelles fonctionnalités
- Optimiser les performances

Voir [CONTRIBUTING.md](CONTRIBUTING.md) et `docs/` pour plus d'infos.

## 📝 Licence

Ce projet est sous licence MIT - voir [LICENSE](LICENSE)

## 👨‍💻 Contribution

Les contributions sont les bienvenues! Consultez [CONTRIBUTING.md](CONTRIBUTING.md)

## 📧 Contact

Pour des questions ou suggestions, créez une issue sur GitHub.

---
**BIXXY Bike Backlight** - Faire briller votre vélo intelligemment ✨
