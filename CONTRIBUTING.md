# Contribution au Projet BIXXY

Merci de votre intérêt pour contribuer à BIXXY Bike Backlight! 🙏

## 📋 Guide de Contribution

### 1. Avant de Commencer

- Lisez [ARCHITECTURE.md](ARCHITECTURE.md)
- Consultez [MODULES.md](docs/MODULES.md)
- Comprenez les [conventions de code](ARCHITECTURE.md#-conventions-de-code)

### 2. Forker et Cloner

```bash
# Fork le repo sur GitHub
# Cloner votre fork
git clone https://github.com/YOUR_USERNAME/bixxy-bike-backlight.git
cd bixxy-bike-backlight

# Ajouter upstream
git remote add upstream https://github.com/ORIGINAL_OWNER/bixxy-bike-backlight.git
```

### 3. Créer une Branche

```bash
# Créer une branche pour votre feature
git checkout -b feature/nom-feature

# Ou pour une correction
git checkout -b fix/nom-correction

# Ou pour documentation
git checkout -b docs/nom-doc
```

### 4. Modifications de Code

#### Structure à Respecter

Pour chaque nouveau module:
```
src/mon_module/
├── mon_module.h      # Interface publique
└── mon_module.c      # Implémentation
```

#### Template d'un Header (`.h`)

```c
#ifndef MON_MODULE_H
#define MON_MODULE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Description du module
 * @details Détails additionnels
 */

// Types publics
typedef struct {
    // Données
} mon_module_data_t;

// Fonctions publiques

/**
 * @brief Initialiser le module
 * @return true si succès, false sinon
 */
bool mon_module_init(void);

/**
 * @brief Lire les données
 * @param data Pointeur vers structure de résultats
 * @return true si succès
 */
bool mon_module_read(mon_module_data_t *data);

/**
 * @brief Deinitializer le module
 */
void mon_module_deinit(void);

#endif // MON_MODULE_H
```

#### Template d'une Implémentation (`.c`)

```c
#include "mon_module.h"
#include "../config.h"
#include <stdio.h>

// Constantes privées
#define MON_MODULE_TIMEOUT 1000  // ms

// Types privés
typedef struct {
    bool initialized;
    // État interne
} mon_module_state_t;

// Variables privées
static mon_module_state_t state = {0};

// Fonctions privées
static bool _validate_data(const mon_module_data_t *data);

// Implémentation
bool mon_module_init(void) {
    if (state.initialized) {
        return true;
    }
    
    // Initialisation du hardware
    
    state.initialized = true;
    return true;
}

bool mon_module_read(mon_module_data_t *data) {
    if (!state.initialized || !data) {
        return false;
    }
    
    // Lire les données
    
    if (!_validate_data(data)) {
        return false;
    }
    
    return true;
}

void mon_module_deinit(void) {
    state.initialized = false;
}

static bool _validate_data(const mon_module_data_t *data) {
    // Validation logique
    return true;
}
```

### 5. Commit et Push

```bash
# Ajouter les fichiers modifiés
git add src/mon_module/*

# Commit avec message clair
git commit -m "feature: Ajouter nouveau module GPS v2"
# ou
git commit -m "fix: Correction bug dans LED control"
# ou
git commit -m "docs: Mise à jour guide installation"

# Push vers votre fork
git push origin feature/nom-feature
```

#### Format des Messages de Commit

```
<type>: <description courte>

<description détaillée optionnelle>

- Point 1
- Point 2

Fixes #123
```

Types acceptés:
- `feature` - Nouvelle fonctionnalité
- `fix` - Correction de bug
- `docs` - Documentation uniquement
- `refactor` - Refactorisation sans changement fonctionnel
- `test` - Ajout de tests
- `perf` - Amélioration de performance

### 6. Pull Request

1. Allez sur GitHub et créez une PR
2. Titre descriptif
3. Description de changements
4. Reference aux issues (`Fixes #123`)
5. Screenshots/vidéos si pertinent

```markdown
## Description
Ajouter le module GPS v2 avec support GNSS

## Type de Changement
- [x] Nouvelle fonctionnalité
- [ ] Correction de bug
- [ ] Breaking change
- [ ] Documentation

## Tests
- [x] Testé sur Arduino Uno
- [x] Testé sur ESP32
- [ ] Test unitaire inclus

## Checklist
- [x] Code suit les conventions du projet
- [x] Documentation mise à jour
- [x] Pas de warnings à la compilation
```

## 🎯 Domaines de Contribution

### Modules à Développer/Améliorer
- [ ] Module GPS (parsing NMEA optimisé)
- [ ] Accéléromètre (algorithme de fusion)
- [ ] LEDs (nouveaux patterns)
- [ ] Cloud (protocoles MQTT/CoAP)
- [ ] Capteurs (capteur température, distance)

### Documentation
- [ ] Tutoriels de démarrage
- [ ] FAQ
- [ ] Schémas électroniques
- [ ] Vidéos de démonstration

### Tests
- [ ] Tests unitaires
- [ ] Tests d'intégration
- [ ] Benchmarks de performance

### Optimisations
- [ ] Réduction de la consommation RAM
- [ ] Optimisation des timers
- [ ] Réduction de latence

## 🐛 Signaler un Bug

Créez une issue avec:

```markdown
**Describe the bug**
Description claire du problème

**To Reproduce**
1. Étape 1
2. Étape 2
3. Comportement attendu

**Environment**
- Platform: [Arduino Uno / Mega / ESP32]
- Arduino IDE version / PlatformIO
- Librairies utilisées
```

## ❓ Questions et Discussions

- Ouvrez une Discussion sur GitHub
- Poser des questions avant de commencer un gros travail
- Partagez vos idées!

## 📜 Accord de Contributor

En contribuant, vous acceptez que:
- Votre code soit publié sous licence MIT
- Votre nom soit crédité si demandé
- Le projet peut utiliser votre code comme base d'améliorations futures

## 🙏 Remerciements

Tous les contributeurs sont appréciés! Votre aide rend BIXXY meilleur chaque jour.

---
**Heureux de coder ensemble!** 🚀
