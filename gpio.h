/**
 ***************************************************************************
 * @brief   CAS EBX-FS15 Embedded Linux \n
 *	    Implements GPIO functions, e.g.: \n
 *	    export / unexport / set output / input.\n
 * @file    gpio.h
 * @version 1.0
 * @date    09.05.2015
 * @author  Michel Grundmann
 *
 * @remark  Last Modifications:
 * @remark  V1.0, mg, 09.05.2015
 ***************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Makes the specific GPIO available for direct interaction.
 * @param [in] inGpioNumber: GPIO number that has to be made available.
 */
void gpio_export(const int inGpioNumber);
/**
 * Frees the specific GPIO.
 * @param [in] inGpioNumber: GPIO number that has to be released.
 */
void gpio_unexport(const int inGpioNumber);
/**
 * Sets the GPIO to output.
 * @param [in] inGpioNumber: GPIO number that has to be set to output.
 */
void gpio_setToOutput(const int inGpioNumber);
/**
 * Sets the GPIO to input.
 * @param [in] inGpioNumber: GPIO number that has to be set to output.
 */
void gpio_setToInput(const int inGpioNumber);
/**
 * Sets the GPIO to 1.
 * @param [in] inGpioNumber: GPIO number that has to be set.
 */
void gpio_set(const int inGpioNumber);
/**
 * Gets the GPIO value.
 * @param [in]  inGpioNumber: GPIO number to get the value from.
 * @param [out] outValueP: Pointer to write the returned value to.
 */
void gpio_get(const int inGpioNumber, int* outValueP);
/**
 * Sets the GPIO to 0.
 * @param [in] inGpioNumber: GPIO number that has to be cleared.
 */
void gpio_clear(const int inGpioNumber);


#ifdef __cplusplus
}
#endif

