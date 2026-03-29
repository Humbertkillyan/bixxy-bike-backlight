# Architecture BIXXY Bike Backlight

## 🏗️ Vue d'ensemble structurelle

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
    │    SENSORS        │
    │    Module         │
    └────────┬──────────┘
             │
    ┌────────▼──────────┐
    │    CLOUD          │
    │    Module         │
    └───────────────────┘
```

## 📁 Structure des Répertoires

### `src/`
Code source principal du projet

#### `main.c / main.h`
- Point d'entrée de l'application
- Boucle principale
- Orchestration des modules
- Gestion des états globaux

#### `config.h`
- Configuration centralisée
- Constantes globales
- Paramètres de compilation
- PIN definitions

#### `gps/`
- `gps.h` - Interface du module GPS
- `gps.c` - Implémentation
- Responsabilités:
  - Initialisation du module GPS
  - Lecture des coordonnées
  - Parsing des données NMEA
  - Validation des données

#### `accelerometer/`
- `accelerometer.h` - Interface
- `accelerometer.c` - Implémentation
- Responsabilités:
  - Communication I2C/SPI
  - Calibration
  - Lecture des axes X/Y/Z
  - Détection de mouvements

#### `leds/`
- `leds.h` - Interface
- `leds.c` - Implémentation
- Responsabilités:
  - Contrôle des broches GPIO
  - Gestion PWM
  - Patterns d'animation
  - Intensité adaptative

#### `cloud/`
- `cloud.h` - Interface
- `cloud.c` - Implémentation
- Responsabilités:
  - Communication WiFi/Connexion réseau
  - Format et envoi de données
  - Réception de commandes
  - Gestion des erreurs réseau

#### `sensors/`
- `sensors.h` - Interface générique
- `sensors.c` - Implémentation
- Responsabilités:
  - Agrégation de capteurs
  - Fusion de données
  - Filtrage et moyennes
  - Logs locaux

#### `utils/`
- `utils.h` - Interface
- `utils.c` - Implémentation
- Responsabilités:
  - Fonctions mathématiques
  - Gestion du temps
  - Utilitaires de chaînes
  - Logging

### `include/`
Headers publics réutilisables si nécessaire

### `docs/`
Documentation détaillée:
- `SETUP.md` - Installation et configuration
- `MODULES.md` - Documentation technique
- `API.md` - Référence API complète
- `DEVELOPMENT.md` - Guide développement
- `CLOUD_INTEGRATION.md` - Intégration cloud
- `AI_INSTRUCTIONS.md` - Instructions pour agents IA

### `test/`
Tests unitaires et de validation

## 🔄 Flux de Données Typique

1. **Initialisation** (setup)
   - `main.c` initialise tous les modules
   - Configuration GPIO et interfaces

2. **Boucle principale** (loop)
   - Lecture GPS
   - Lecture accéléromètre
   - Traitement capteurs
   - Décisions LED
   - Synchronisation cloud (périodique)

3. **Communication**
   ```
   GPS Module → Sensors Module → Cloud Module
   Accelerometer Module → Sensors Module → LED Module
   ```

## 💾 Conventions de Code

### Nommage des Fichiers
- `module_name.h` - Header
- `module_name.c` - Implémentation

### Nommage des Fonctions
```c
// Public (dans le .h)
module_function_name()

// Privé (dans le .c)
_module_private_function()
```

### Structures et Types
```c
// Types spécifiques au module
typedef struct {
    int x, y, z;
} accel_data_t;

// Enums pour états
typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_ERROR
} module_state_t;
```

## 🌐 Intégration Cloud

```
Device (Arduino)
    ↓
    ├── Collecte données (GPS, Accél, Capteurs)
    ├── Formatage JSON
    └── Envoi via WiFi/Connexion
            ↓
        Backend Cloud
            ├── Stockage
            ├── Traitement
            ├── Analytics
            └── Commandes retour
                    ↓
        Device (Mise à jour configs, LED patterns, etc)
```

## 📈 Cycle de Vie d'un Module

1. **Initialisation** - `module_init()`
2. **Configuration** - `module_configure()`
3. **Opération** - `module_read()` / `module_write()`
4. **Monitoring** - `module_status()`
5. **Cleanup** - `module_deinit()`

## 🚦 Gestion des États

```
UNINITIALIZED
    ↓
IDLE ←→ ACTIVE ←→ PROCESSING
    ↓
ERROR → RECOVERY → IDLE
```

## 🔐 Sécurité et Robustesse

- Vérification des erreurs à chaque appel
- Timeouts pour opérations réseau
- Validation des données reçues
- Récupération des erreurs gracieuse
- Watchdog timer pour réinitialisation

## 📊 Performance

- Lecture GPS: ~1Hz
- Lecture accéléromètre: ~50Hz (configurable)
- Mise à jour LED: ~60Hz
- Upload cloud: ~1 fois/10s (configurable)
- RAM: <10KB pour modules principaux

---
*Documentation mise à jour pour version 1.0*
