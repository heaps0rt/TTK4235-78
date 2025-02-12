#ifndef SYSTEM_H
#define SYSTEM_H

/**
 * @file
 * @brief Enumeration of different PID tuning methods.
**/

typedef enum {
    Classic_Ziegler_Nichols,
    Pessen_Integral_Rule
} Tuning;

/**
 * @brief A structure to represent a system and desired PID tuning method.
 */
typedef struct {
    double Ku;            /** < The gain margin for loop stability */
    double Tu;            /** < The period of the oscillations frequency at the stability limit */
    Tuning tuning_method; /** < The desired PID tuning method */
} System;


/**
 * @brief Creates a new system instance.
 *
 * @param Ku
 * @param Tu
 * @param tuning_method The selected PID tuning method.
 * @return A pointer to the newly created `System` instance.
 */
System * system_create(double Ku, double Tu, Tuning tuning_method);

/**
 * @brief Deletes a system instance and frees allocated memory.
 *
 * @param system_current Pointer to the `System` instance to be deleted.
 */
void system_delete(System * system_current);

#endif
