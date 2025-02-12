#ifndef CALCULATIONS_H
#define CALCUATIONS_H
#include "system.h"

/**
* @file
* @brief A simple PID controller
*/

/**
 * @brief Calculates the proportional (P) parameter for a PID controller.
 *
 * This function determines the P gain
 *
 * @param current_system Pointer
 * @return The calculated P parameter.
 */

double calculate_P_parameter(System * current_system);

/**
 * @brief Calculates the proportional (I) parameter for a PID controller.
 *
 * This function determines the I gain
 *
 * @param current_system Pointer
 * @return The calculated I parameter.
 */
double calculate_I_parameter(System * current_system);

/**
 * @brief Calculates the proportional (I) parameter for a PID controller.
 *
 * This function determines the I gain
 *
 * @param current_system Pointer
 * @return The calculated I parameter.
 */
double calculate_D_parameter(System * current_system);

#endif
