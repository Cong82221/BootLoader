#include "MKE16Z4.h"
#include "HAL_GPIO.h"

/******************************************************************************* * Definitions ******************************************************************************/
#define GREEN_LED_PIN (1 << 4) /* Define pin number Green LED is attached to */
#define RED_LED_PIN (1 << 5)   /* Define pin number Red LED is attached to */
#define BLUE_LED_PIN (1 << 1)  /* Define pin number Blue LED is attached to */
#define SW2 (1 << 3)           /* Define pin number Switch 2 is attached to */
#define SW3 (1 << 2)           /* Define pin number Switch 3 is attached to */

/******************************************************************************* * Prototypes ******************************************************************************/

/******************************************************************************* * Variables ******************************************************************************/

/******************************************************************************* * Code ******************************************************************************/
void HAL_GPIO_InitButton(HAL_GPIO_Switch_Type_t switchType, HAL_GPIO_Interrupt_Type_t interruptType)
{
    /* Provide clock for PORTD */
    PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC(1);
    if (interruptType != DisableIRQC)
    {
        NVIC_EnableIRQ(PORTBCD_IRQn);
    }
    else
    {
        NVIC_DisableIRQ(PORTBCD_IRQn);
    }
    switch (switchType)
    {
    case Switch2:
        /* Set GPIO input for SW2 */
        PORTD->PCR[3] = PORT_PCR_MUX(1);
        FGPIOD->PDDR &= ~SW2;

        /* Set Pullup input for SW2 */
        PORTD->PCR[3] |= PORT_PCR_PE(1);
        PORTD->PCR[3] |= PORT_PCR_PS(1);

        /* Set Interrupt for SW2 */
        PORTD->PCR[3] |= PORT_PCR_IRQC(interruptType);
        break;

    case Switch3:
        /* Set GPIO input for SW3 */
        PORTD->PCR[2] = PORT_PCR_MUX(1);
        FGPIOD->PDDR &= ~SW3;

        /* Set Pullup input for SW3 */
        PORTD->PCR[2] |= PORT_PCR_PE(1);
        PORTD->PCR[2] |= PORT_PCR_PS(1);

        /* Set Interrupt for SW3 */
        PORTD->PCR[2] |= PORT_PCR_IRQC(interruptType);
        break;

    default:
        break;
    }
}

void HAL_GPIO_InitLed(void)
{
    /* Enable Clock for PORTB */
    PCC->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC(1);

    /* Setup PINB4, PINB5 as GPIO */
    PORTB->PCR[4] = PORT_PCR_MUX(1);
    PORTB->PCR[5] = PORT_PCR_MUX(1);

    /* Setup PINB4 as Output Mode */
    FGPIOB->PDDR |= GREEN_LED_PIN | RED_LED_PIN;

    /* Enable Clock for PORTD */
    PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC(1);

    /* Setup PIND1 as GPIO */
    PORTD->PCR[1] = PORT_PCR_MUX(1);

    /* Setup PIND1 as Output Mode */
    FGPIOD->PDDR |= BLUE_LED_PIN;
}

void HAL_GPIO_SetPin(HAL_GPIO_LED_Type_t ledType)
{
    if (ledType == Red)
    {
        FGPIOB->PSOR |= RED_LED_PIN;
    }
    else if (ledType == Green)
    {
        FGPIOB->PSOR |= GREEN_LED_PIN;
    }
    else if (ledType == Blue)
    {
        FGPIOD->PSOR |= BLUE_LED_PIN;
    }
    else
    {
    }
}

void HAL_GPIO_ClearPin(HAL_GPIO_LED_Type_t ledType)
{
    if (ledType == Red)
    {
        FGPIOB->PCOR |= RED_LED_PIN;
    }
    else if (ledType == Green)
    {
        FGPIOB->PCOR |= GREEN_LED_PIN;
    }
    else if (ledType == Blue)
    {
        FGPIOD->PCOR |= BLUE_LED_PIN;
    }
    else
    {
    }
}

void HAL_GPIO_TogglePin(HAL_GPIO_LED_Type_t ledType)
{
    if (ledType == Red)
    {
        FGPIOB->PTOR |= RED_LED_PIN;
    }
    else if (ledType == Green)
    {
        FGPIOB->PTOR |= GREEN_LED_PIN;
    }
    else if (ledType == Blue)
    {
        FGPIOD->PTOR |= BLUE_LED_PIN;
    }
    else
    {
    }
}

HAL_GPIO_Switch_Type_t HAL_GPIO_ReadISF(void)
{
    HAL_GPIO_Switch_Type_t switchType = Switch2;

    if ((uint32_t)(PORTD->ISFR) == (uint32_t)SW3)
    {
        switchType = Switch3;
    }
    else if ((uint32_t)(PORTD->ISFR) == (uint32_t)SW2)
    {
        switchType = Switch2;
    }
    else
    {
    }
    return switchType;
}

void HAL_GPIO_ClearISF(HAL_GPIO_Switch_Type_t switchType)
{
    if (switchType == Switch2)
    {
        PORTD->ISFR |= SW2;
    }
    else if (switchType == Switch3)
    {
        PORTD->ISFR |= SW3;
    }
    else
    {
    }
}