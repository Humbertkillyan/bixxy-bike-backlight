/**
 * @file main.h
 * @brief Point d'entrée principal d'application BIXXY
 * @details Orchestration de tous les modules et boucle principale
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialiser tous les modules
 * @return true si tous OK, false sinon
 */
bool system_init(void);

/**
 * @brief Boucle principale - à appeler continuellement
 */
void system_loop(void);

/**
 * @brief Arrêter tous les modules
 */
void system_shutdown(void);

#endif // MAIN_H
