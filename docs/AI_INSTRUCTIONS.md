# Instructions pour Agents IA

Ce document explique comment les agents IA (comme Claude, ChatGPT, etc.) peuvent interagir avecle projet BIXXY.

## 🤖 Comprendre l'Architecture

### Structure Générale

```
BIXXY = Collection de modules indépendants
Chaque module = 1 interface (.h) + 1 implémentation (.c)
Aucune interdépendance entre modules (sauf vers utils)
main.c = orchestration des modules
```

### Pattern de Module Standard

**TOUJOURS** respecter ce pattern:

```
src/mon_module/
├── mon_module.h       # Interface publique
│   ├── Types publics (struct, enum)
│   └── Déclarations fonctions publiques
│
└── mon_module.c       # Implémentation
    ├── Constants privées (#define)
    ├── Types privés
    ├── Variables statiques (état privé)
    ├── Fonctions privées (_mon_func)
    └── Implémentation des fonctions publiques
```

## 📋 Tasks Typiques pour Agents IA

### 1. Ajouter un Nouveau Capteur

**Demande du user**: "Ajoute un capteur de température DHT22"

**Processus**:

1. Créer `src/temperature/temperature.h`
   - Définir structure de données
   - Déclarer fonctions publiques

2. Créer `src/temperature/temperature.c`
   - Implémenter avec patterns standard
   - Gestion d'erreurs robuste
   - Logs appropriés

3. Intégrer dans `src/sensors/sensors.c`
   - Ajouter initialisation
   - Ajouter dans fusion de données

4. Documenter
   - Ajouter dans `docs/MODULES.md`
   - Ajouter dans `docs/API.md`

5. Mettre à jour `main.c`
   - Ajouter l'initialisation si nécessaire

### 2. Implémenter une Fonctionnalité Cloud

**Demande du user**: "Implémente la synchronisation MQTT"

**Vérifications importantes**:

- [ ] Ne pas bloquer la boucle principale
- [ ] Non-bloquant avec timeouts
- [ ] Gestion d'erreurs réseau
- [ ] Validation des données reçues
- [ ] Logs des opérations critiques

### 3. Ajouter un Pattern LED

**Demande du user**: "Ajoute pattern LED du feu de route"

**Responsabilités**:

- [ ] Définir le pattern dans `leds.h`
- [ ] Implémenter dans `leds.c`
- [ ] Ajouter dans l'enum `led_mode_t`
- [ ] Tester les performances RAM/CPU

## 🎯 Checklist avant de Coder

### Avant Tout Changement:

- [ ] Lire [ARCHITECTURE.md](../ARCHITECTURE.md)
- [ ] Consulter [MODULES.md](MODULES.md)
- [ ] Vérifier [DEVELOPMENT.md](DEVELOPMENT.md)
- [ ] Chercher code existant similaire

### Code Review Interne:

```
Pour chaque modification, s'assurer:

✓ Compile sans warning
✓ Pas de dépendances circulaires
✓ Interface publique claire
✓ Gestion d'erreurs complète
✓ Pas de malloc/free en loop
✓ Timing prévisible
✓ Logs appropriés
✓ Code commenté si non-obvious
✓ Docstrings en place
✓ Pas de global non-static
```

## 🔍 Patterns et Conventions

### Nommage

```c
// Public - complet et clair
bool gps_read(gps_data_t *data);

// Privé - underscore prefix
static void _gps_handle_uart_error(void);

// Type - suffix _t
typedef struct { ... } mon_data_t;
typedef enum { ... } mon_state_t;

// Constantes - UPPER_CASE
#define GPS_TIMEOUT_MS 1000
```

### Template Fonction Complète

Chaque fonction doit avoir:

```c
/**
 * @brief Une ligne descriptive
 * @param param Description
 * @return Documentation du retour
 * @note Remarques importantes
 */
bool mon_module_operation(int param) {
    // 1. Valider les paramètres
    if (param < 0) {
        utils_log("ERROR", "Invalid parameter");
        return false;
    }
    
    // 2. Exécuter l'opération
    // ...
    
    // 3. Logger les résultats
    utils_log("DEBUG", "Operation completed");
    
    // 4. Retourner le statut
    return true;
}
```

### Gestion État Module

```c
// État privé toujours statique (jamais global)
static mon_module_state_t state = {0};

// Vérifier initialisation
bool mon_module_read(mon_module_data_t *data) {
    if (!state.initialized || !data) {
        return false;
    }
    // ...
}

// Toujours nettoyer
void mon_module_deinit(void) {
    state.initialized = false;
    // cleanup code
}
```

## 📈 Tasks Complexes

### Situation: Améliorer l'Accéléromètre

**Demande réaliste**:
> "L'accéléromètre a trop de bruit, ajoute un filtre passe-bas et améliore la calibration"

**Analyse par agent IA**:

1. Localiser le code: `src/accelerometer/accelerometer.c`
2. Comprendre structure actuelle
3. Ajouter type pour filtre
4. Implémenter Kalman ou filtre exponentiel
5. Améliorer calibration
6. Tester performances RAM
7. Documenter les paramètres du filtre
8. Ajouter dans la doc
9. Commit méthodique

### Situation: Intégrer Nouveau Cloud

**Demande réaliste**:
> "On passe d'HTTP à MQTT, réimplémente le module cloud"

**Analyse par agent IA**:

1. Vérifier interface existante
2. Adapter au nouveau protocole
3. MaintEnir la même interface publique (si possible)
4. Tester reconnexion
5. Logs détaillés des étapes
6. Pas d'impact sur autres modules
7. Mettre à jour la doc
8. Commit

## 🚫 Antipatterns à Éviter

### ❌ Ajouter dépendance circulaire

```c
// MAUVAIS:
// gps.h #includes accelerometer.h
// accelerometer.h #includes gps.h
// → Erreur de compilation ou comportement indéfini
```

**Correct**:
- Modules indépendants
- Communication via `sensors.c`
- Seulement `#include "../utils/utils.h"`

### ❌ Variables Globales Mutable

```c
// MAUVAIS:
int global_counter = 0;  // Shared state partout!

// BON:
static mon_state_t state = {0};  // Privé au module
```

### ❌ Opérations Bloquantes

```c
// MAUVAIS:
while (!data_available) { }  // Bloque tout!

// BON:
uint32_t deadline = millis() + TIMEOUT;
while (!data_available && millis() < deadline) {
    // other code
}
```

### ❌ Pas de Validation

```c
// MAUVAIS:
void mon_func(int *ptr, const char *str) {
    *ptr = 42;        // Crash si ptr == NULL
    strcpy(buf, str); // Buffer overflow possible
}

// BON:
bool mon_func(int *ptr, const char *str) {
    if (!ptr || !str) return false;
    // Validation additionnelle
    *ptr = 42;
    return true;
}
```

## 🧠 Logique de Réflexion pour Agents

### Avant d'Écrire du Code:

1. **Comprendre le problème**
   - Lire la demande du user
   - Regarder le code existant
   - Identifier le pattern à utiliser

2. **Vérifier les Dépendances**
   - Quels modules sont affectés?
   - Besoin de modification dans `main.c`?
   - Nouveaux includes nécessaires?

3. **Planifier la Solution**
   - Structure des fichiers
   - Nouvelles functions publiques
   - État privé requis

4. **Implémenter Idempotent**
   - Code peut être exécuté 2x sans problème
   - Idempotence des init/deinit

5. **Tester Mentalement**
   - Compilation?
   - Comportement attendu?
   - Cas d'erreur?

6. **Documenter**
   - Code comments
   - Update MODULES.md
   - Update API.md

7. **Commit Logiquement**
   - PR cohérente
   - Message clair
   - Une fonctionnalité par PR

## 📚 Ressources dans le Repo

Pour agents IA travaillant sur BIXXY:

1. **Architecture**: Lire d'abord `ARCHITECTURE.md`
2. **Coding**: Consulter `DEVELOPMENT.md`
3. **API**: Vérifier `docs/API.md`
4. **Modules**: Étudier `docs/MODULES.md`
5. **Setup**: Connaître `docs/SETUP.md`
6. **Contribution**: Respecter `CONTRIBUTING.md`

## 🔧 Commandes Utiles pour Agents

```bash
# Compiler sans upload
platformio run -e arduino_uno

# Compiler et upload
platformio run --target upload -e arduino_uno

# Surveiller série
platformio device monitor -b 115200

# Lancer les tests
platformio test

# Formater le code (si clang-format disponible)
clang-format -i src/**/*.c src/**/*.h
```

## 💾 Structure de Fichiers Attendue

```
TOUJOURS organiser ainsi:

src/
├── main.c
├── config.h
├── mon_module/
│   ├── mon_module.h
│   └── mon_module.c
└── autres_modules/

include/
└── (headers publiques si nécessaire)

docs/
├── SETUP.md
├── MODULES.md
├── API.md
└── AI_INSTRUCTIONS.md (ce fichier)

test/
└── test_*.c

README.md
ARCHITECTURE.md
CONTRIBUTING.md
```

---

## 🎬 Exemple Complet: Ajouter Capteur Luminosité

**User demande**: "Ajoute un capteur de luminosité LDR"

**Agent IA pense**:
```
1. Créer src/light_sensor/ avec .h et .c
2. Interface simple: light_sensor_read() → lux_value
3. Intégrer dans sensors.c dans fusion
4. Ajouter MODULES.md et API.md
5. Intégrer dans loop() de main.c si nécessaire
6. Pas de dépendance circulaire ✓
7. Gestion d'erreurs I2C ✓
8. Logs appropriés ✓
```

**Agent IA implémente**:
```c
// src/light_sensor/light_sensor.h
typedef struct {
    uint16_t lux;
    uint32_t timestamp;
} light_sensor_data_t;

bool light_sensor_init(void);
bool light_sensor_read(light_sensor_data_t *data);
void light_sensor_deinit(void);

// src/light_sensor/light_sensor.c
// Implémentation approche, tests, doc...
```

---

*Instructions pour Agents IA - Version 1.0 - Mars 2026*
