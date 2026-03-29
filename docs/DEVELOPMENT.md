# Guide de Développement

## 🎯 Principes de Développement BIXXY

### 1. Modularité Extrême
Chaque composant doit:
- Avoir sa propre interface (`.h`)
- Être compilable indépendamment
- Avoir zéro dépendances externes (sauf utils)
- Être testable en isolation

### 2. Interfaces Claires
```c
// BON - Interface claire et prévisible
bool gps_init(void);
bool gps_read(gps_data_t *data);

// MAUVAIS - Retours ambigus, effets de bord
int gps_read(void);  // Quel int? Comment interpréter?
```

### 3. Gestion d'Erreurs
```c
// Toujours vérifier les retours
if (!gps_init()) {
    Serial.println("GPS init failed!");
    return;
}

// Jamais d'assertions silencieuses
// TOUJOURS logger les erreurs
```

### 4. Performance en Temps Réel
```c
// Jamais de malloc/free en loop
// Jamais de fichiers bloquants
// Timeouts sur opérations réseau
// Predictible memory footprint
```

---

## 🏗️ Structure d'un Module

### Arborescence Minimale

```
src/mon_module/
├── mon_module.h      (interface publique)
└── mon_module.c      (implémentation)
```

### Template Header (.h)

```c
/**
 * @file mon_module.h
 * @brief Description du module
 * @details Détails plus longs
 * 
 * Responsabilités:
 * - Tâche 1
 * - Tâche 2
 */

#ifndef MON_MODULE_H
#define MON_MODULE_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// TYPES PUBLIC
// ============================================================================

typedef struct {
    uint32_t field1;
    float field2;
} mon_module_data_t;

typedef enum {
    STATE_INIT,
    STATE_READY,
    STATE_ERROR
} mon_module_state_t;

// ============================================================================
// INTERFACE PUBLIQUE
// ============================================================================

/**
 * @brief Initialiser le module
 * @details Description détaillée si nécessaire
 * @return true si succès, false sinon
 * @note À appeler une seule fois au démarrage
 */
bool mon_module_init(void);

/**
 * @brief Lire les données
 * @param data Pointeur vers structure de résultats
 * @return true si lecture OK, false sinon
 */
bool mon_module_read(mon_module_data_t *data);

/**
 * @brief Reset le module
 * @return true si succès
 */
bool mon_module_reset(void);

/**
 * @brief Arrêter le module
 */
void mon_module_deinit(void);

#endif // MON_MODULE_H
```

### Template Source (.c)

```c
/**
 * @file mon_module.c
 * @brief Implémentation du module
 */

#include "mon_module.h"
#include "../config.h"
#include "../utils/utils.h"
#include <stdio.h>

// ============================================================================
// CONSTANTS PRIVÉES
// ============================================================================

#define MON_MODULE_TIMEOUT_MS   1000
#define MON_MODULE_RETRY_COUNT  3

// ============================================================================
// TYPES PRIVÉS
// ============================================================================

typedef struct {
    bool initialized;
    uint32_t last_read;
    uint8_t error_count;
} mon_module_state_t;

// ============================================================================
// VARIABLES PRIVÉES
// ============================================================================

static mon_module_state_t state = {0};

// ============================================================================
// FONCTIONS PRIVÉES (déclarations)
// ============================================================================

static bool _validate_data(const mon_module_data_t *data);
static void _handle_error(void);
static void _reset_state(void);

// ============================================================================
// INTERFACE PUBLIQUE (implémentation)
// ============================================================================

bool mon_module_init(void) {
    if (state.initialized) {
        return true;
    }
    
    utils_log("INFO", "Initializing MON_MODULE");
    
    // Étape 1: Setup hardware
    // (code spécifique)
    
    // Étape 2: Vérifier disponibilité
    // if (!check_available()) {
    //     utils_log("ERROR", "Hardware not available");
    //     return false;
    // }
    
    state.initialized = true;
    state.error_count = 0;
    
    return true;
}

bool mon_module_read(mon_module_data_t *data) {
    if (!state.initialized || !data) {
        utils_log("ERROR", "Module not initialized or invalid param");
        return false;
    }
    
    // Lire les données
    mon_module_data_t temp = {0};
    
    // Logique de lecture
    // (code spécifique)
    
    if (!_validate_data(&temp)) {
        state.error_count++;
        if (state.error_count > MON_MODULE_RETRY_COUNT) {
            _handle_error();
        }
        return false;
    }
    
    // Copier les données valides
    *data = temp;
    state.error_count = 0;
    state.last_read = millis();
    
    return true;
}

bool mon_module_reset(void) {
    if (!state.initialized) {
        return false;
    }
    
    _reset_state();
    return true;
}

void mon_module_deinit(void) {
    if (state.initialized) {
        utils_log("INFO", "Stopping MON_MODULE");
        // Cleanup
    }
    
    _reset_state();
}

// ============================================================================
// FONCTIONS PRIVÉES (implémentation)
// ============================================================================

static bool _validate_data(const mon_module_data_t *data) {
    if (!data) {
        return false;
    }
    
    // Validations métier
    // (code spécifique)
    
    return true;
}

static void _handle_error(void) {
    utils_log("ERROR", "MON_MODULE: Too many errors, resetting");
    _reset_state();
    mon_module_init();
}

static void _reset_state(void) {
    state.initialized = false;
    state.last_read = 0;
    state.error_count = 0;
}
```

---

## 🧵 Cycle de Vie d'un Module

```
┌───────────────────┐
│   Not Loaded      │
└────────┬──────────┘
         │
         │ init()
         ↓
    ┌─────────────────┐
    │   Initialized   │◄─────┐
    │   (Ready)       │      │
    └────────┬────────┘      │
             │               │
          read()         reset()
             │               │
             ↓               │
    ┌─────────────────┐      │
    │   Processing    │      │
    │   Data          │──────┘
    └────────┬────────┘
             │
             │
         deinit()
             │
             ↓
    ┌─────────────────┐
    │   Deactivated   │
    └─────────────────┘
```

---

## 🧪 Testing et Validation

### Tests Unitaires

```c
// test/test_mon_module.c
#include <unity.h>
#include "../src/mon_module/mon_module.h"

void setUp(void) {
    // Avant chaque test
}

void tearDown(void) {
    // Après chaque test
    mon_module_deinit();
}

void test_mon_module_init_success(void) {
    TEST_ASSERT_TRUE(mon_module_init());
}

void test_mon_module_read_returns_false_when_not_initialized(void) {
    mon_module_data_t data;
    TEST_ASSERT_FALSE(mon_module_read(&data));
}

void test_mon_module_read_success(void) {
    TEST_ASSERT_TRUE(mon_module_init());
    
    mon_module_data_t data;
    TEST_ASSERT_TRUE(mon_module_read(&data));
    TEST_ASSERT_GREATER_THAN(0, data.field1);
}
```

### Compilation et Exécution

```bash
# Compiler le test
platformio test

# Ou individuellement
platformio test -k test_mon_module
```

---

## 📚 Documentation du Code

### Commentaires

```c
// BON - Explique le POURQUOI
// Utiliser un timeout plutôt que bloquage pour éviter
// les watchdog timeouts sur Arduino
uint32_t deadline = millis() + TIMEOUT_MS;

// MAUVAIS - Explique l'OBVIOUS
// Incrémenter i
i++;

// BON - Commentaires en haut de section
// ============================================================================
// GPS COMMUNICATION SETUP
// ============================================================================

// MAUVAIS - Trop de commentaires
/* Ceci est une boucle for
   Elle iterate de 0 à 10
   Elle execute le code à chaque itération
*/
for (int i = 0; i < 10; i++) {
    // code
}
```

### Documentation Fonctions

```c
/**
 * @brief Une ligne description
 * 
 * @details Description détaillée et comportement
 * de la fonction sur plusieurs lignes si nécessaire.
 * 
 * @param param1 Description du paramètre 1
 * @param param2 Description du paramètre 2
 * 
 * @return Description de la valeur de retour
 * 
 * @retval true Si succès
 * @retval false Si erreur
 * 
 * @note Note importante
 * @warning Avertissement
 * 
 * @see function_related()
 */
bool mon_function(int param1, float param2);
```

---

## 🔄 Best Practices

### ✅ À Faire

```c
// Vérifier les retours
if (!gps_init()) {
    utils_log("ERROR", "GPS init failed");
    return;
}

// Utiliser des types appropriés
#include <stdint.h>
uint32_t timer;
float temperature;

// Initialiser les variables
mon_module_state_t state = {0};

// Utiliser des constantes
#define GPS_UART_BAUD 9600
const uint32_t TIMEOUT_MS = 1000;

// Garder les fonctions simples et courtes
bool simple_function(void) {
    if (!check_condition()) return false;
    perform_action();
    return true;
}

// Utiliser des enums pour états
typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_ERROR
} state_t;

// Non-bloquant
uint32_t deadline = millis() + TIMEOUT;
while (!done && millis() < deadline) {
    // process
}
```

### ❌ À Éviter

```c
// Pas de malloc/free en loop
void loop() {
    char *buffer = malloc(100);  // MAUVAIS!
    // ...
    free(buffer);
}

// Pas de code bloquant
delay(10000);  // Bloque le reste
while (true) { /* ... */ }

// Pas de variables globales (sauf état statique)
int global_counter;  // MAUVAIS! Utiliser static

// Pas de ternaires complexes
// MAUVAIS:
result = condition1 ? (condition2 ? val1 : val2) : val3;

// BON:
if (condition1) {
    result = condition2 ? val1 : val2;
} else {
    result = val3;
}

// Pas de magic numbers
delay(1234);  // Qu'est-ce que 1234?

// BON:
#define STATUS_CHECK_INTERVAL_MS 1234
delay(STATUS_CHECK_INTERVAL_MS);

// Pas d'erreurs silencieuses
gps_read(&data);  // Pas de vérification!

// BON:
if (!gps_read(&data)) {
    utils_log("ERROR", "GPS read failed");
    return false;
}
```

---

## 🚀 Workflow de Développement

### 1. Créer le Module

```bash
# Créer le répertoire
mkdir src/mon_nouveau_module

# Créer les fichiers template
# (voir templates ci-dessus)
```

### 2. Développer l'Interface

```c
// src/mon_nouveau_module/mon_nouveau_module.h
// Définir les types et la signature des fonctions publiques
```

### 3. Implémentation

```c
// src/mon_nouveau_module/mon_nouveau_module.c
// Implémenter les fonctions publiques
```

### 4. Tests

Ajouter les tests dans `test/`

### 5. Documentation

- Ajouter une entry dans `docs/MODULES.md`
- Ajouter la documentation des fonctions dans `docs/API.md`

### 6. Intégration

Intégrer dans `src/main.c`

### 7. Commit

```bash
git add src/mon_nouveau_module/
git add docs/
git commit -m "feature: Add nouveau module"
```

---

## 📊 Debugging

### Serial Monitor

```bash
platformio device monitor -b 115200
```

### Logging

```c
#include "../utils/utils.h"

utils_log("DEBUG", "GPS latitude: 48.856");
utils_log("ERROR", "I2C timeout");
utils_log("WARN", "Battery low");
```

### Memory Check

```c
// En Arduino
extern int __heap_start, *__brkval;
int free_ram() {
    int v;
    return (int) &v - (__brkval == 0 ? 
            (int) &__heap_start : (int) __brkval);
}
```

---

*Guide de développement - Mars 2026*
