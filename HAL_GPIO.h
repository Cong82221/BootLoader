/**
 * @file HAL_GPIO.h
 * @author William Epic
 * @brief General Purpose Input Output (GPIO) Configurations
 * @version 0.1
 * @date 2022-11-16
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef INC_HAL_GPIO_H_
#define INC_HAL_GPIO_H_

/******************************************************************************* * Definitions ******************************************************************************/
/* This enumeration is to define LED Type for ease of use */
typedef enum
{
    Red = 1,
    Green = 2,
    Blue = 3
} HAL_GPIO_LED_Type_t;

/* This enumeration is to define Switch Type for ease of use */
typedef enum
{
    Switch2 = 2,
    Switch3 = 3,
} HAL_GPIO_Switch_Type_t;

/* This enumeration is to define types of interrupt for switches */
typedef enum
{
    DisableIRQC = 0U,
    Logic0IRQC = 8U,
    RisingIRQC = 9U,
    FallingIRQC = 10U,
    EitherIRQC = 11U,
    Logic1IRQC = 12U,
} HAL_GPIO_Interrupt_Type_t;

/******************************************************************************* * API ******************************************************************************/
/**
 * @brief This function is to initialize GPIO configurations for switches
 *
 * @param switchType this parameter is used to determine which switch to be configured
 * @param interruptType this parameter is used to determine which type of interrupt to be configured
 */
void HAL_GPIO_InitButton(HAL_GPIO_Switch_Type_t switchType, HAL_GPIO_Interrupt_Type_t interruptType);

/**
 * @brief This function is to initialize GPIO configurations for LED
 *
 */
void HAL_GPIO_InitLed(void);

/**
 * @brief This function is to set pin output to HIGH
 *
 * @param ledType this parameter is used to determine which pin output to be controlled
 */
void HAL_GPIO_SetPin(HAL_GPIO_LED_Type_t ledType);

/**
 * @brief This function is to set pin output to LOW
 *
 * @param ledType this parameter is used to determine which pin output to be controlled
 */
void HAL_GPIO_ClearPin(HAL_GPIO_LED_Type_t ledType);

/**
 * @brief This function is to toggle pin output
 *
 * @param ledType this parameter is used to determine which pin output to be controlled
 */
void HAL_GPIO_TogglePin(HAL_GPIO_LED_Type_t ledType);

/**
 * @brief This function is to read Interrupt Status Flag Register
 *
 * @return Switch Type
 */
HAL_GPIO_Switch_Type_t HAL_GPIO_ReadISF(void);

/**
 * @brief This function is to clear Interrupt Status Flag of a specific pin
 *
 * @param switchType this parameter is used to determine which Interrupt Status Flag to be cleared
 */
void HAL_GPIO_ClearISF(HAL_GPIO_Switch_Type_t switchType);

#endif /* INC_HAL_GPIO_H_ */