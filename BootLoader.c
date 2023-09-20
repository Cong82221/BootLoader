#include "MKE16Z4.h"
#include "HAL_GPIO.h"
#include "HAL_LPUART0.h"
#include "SREC.h"
#include "Queue.h"
#include "flash.h"

/******************************************************************************* * Definitions ******************************************************************************/
#define MAX_BUFFER_SIZE 100                      /* Maximum line length received through UART */
#define APP_START_ADDRESS ((uint32_t)0x00008000) /* Start address of User Application section */

/* This enumeration is to define modes of boot */
typedef enum
{
    App = 0U,
    Boot = 1U
} Bootloader_Mode_Type_t;

/* This enumeration is to define bootloader error states */
typedef enum
{
    NoError = 0U,
    Error = 1U
} Bootloader_Error_Type_t;

/* This typedef is to define a function pointer */
typedef void (*pFunction)(void);

/******************************************************************************* * Prototypes ******************************************************************************/
/**
 * @brief This function is to initialize UART
 *
 */
static void Bootloader_InitUART(void);

/**
 * @brief This function is to delay the desired milliseconds
 *
 * @param milliseconds this parameter is the desired delay time in milliseconds
 */
static void Bootloader_Delay(uint32_t milliseconds);

/**
 * @brief This function is to run boot mode
 *
 * @return Error code
 */
static Bootloader_Error_Type_t Bootloader_RunBoot(void);

/**
 * @brief This function is to run user application
 *
 */
static void Bootloader_RunApp(void);

/**
 * @brief This function is to write to flash
 *
 * @param address this parameter is to determine the address in the flash in which the data is written on
 * @param data this parameter is the data needed to be written into the flash
 * @param dataByteCount this parameter is to determine the data byte length
 */
static void Bootloader_Flash(uint32_t address, uint8_t data[], uint32_t dataByteCount);

/******************************************************************************* * Variables ******************************************************************************/
static uint8_t sg_Buffer[MAX_BUFFER_SIZE];            /* Buffer to store characters of a line received through UART */
static volatile Bootloader_Mode_Type_t sg_Mode = App; /* Indicate boot mode (Application mode is default) */

/******************************************************************************* * Code ******************************************************************************/
void main()
{
    Bootloader_Error_Type_t bootError = NoError;
    uint8_t index = 0;

    /* Configure switch 2 to GPIO and set interrupt flag at falling edge */
    HAL_GPIO_InitButton(Switch2, FallingIRQC);
    /* Initialize LED */
    HAL_GPIO_InitLed();
    /* Turn off all LEDs */
    HAL_GPIO_SetPin(Red);
    HAL_GPIO_SetPin(Green);
    HAL_GPIO_SetPin(Blue);
    /* Initialize UART */
    Bootloader_InitUART();
    HAL_LPUART0_TransmitString("\r\n--------Welcome to FRDM-KE16Z!--------\r\n", 42);
    HAL_LPUART0_TransmitString("Application commencing in 3 seconds! Press Button 2 to enter Firmware Update Mode.\r\n", 84);
    /* Delay 3 seconds with LED blink each second */
    for (index = 0; index < 6; index++)
    {
        Bootloader_Delay(500);
        HAL_GPIO_TogglePin(Green);
        HAL_GPIO_TogglePin(Blue);
    }
    if (sg_Mode == Boot)
    {
        HAL_LPUART0_TransmitString("--------Firmware Update Mode--------\r\n", 38);
        HAL_LPUART0_TransmitString("Please send S-record file to update.\r\n", 38);
        /* Turn on Blue LED to indicate wait for S-record file */
        HAL_GPIO_SetPin(Red);
        HAL_GPIO_SetPin(Green);
        HAL_GPIO_ClearPin(Blue);
        /* Run Boot mode */
        bootError = Bootloader_RunBoot();
        if (bootError != NoError)
        {
            /* Turn on Red LED to indicate error in flashing process */
            HAL_GPIO_SetPin(Blue);
            HAL_GPIO_SetPin(Green);
            HAL_GPIO_ClearPin(Red);
            HAL_LPUART0_TransmitString("\r\nThere is an error in flashing process, please RESET and try again!\r\n", 70);
        }
        else
        {
            /* Turn on Green LED to indicate success in flashing process */
            HAL_GPIO_SetPin(Red);
            HAL_GPIO_SetPin(Blue);
            HAL_GPIO_ClearPin(Green);
            HAL_LPUART0_TransmitString("\r\nFirmware updated successfully! Please RESET to enter application\r\n", 68);
        }
    }
    else
    {
        HAL_LPUART0_TransmitString("--------Application Initialized!--------\r\n", 42);
        /* Run Application */
        Bootloader_RunApp();
    }
    while (1);
}

static void Bootloader_InitUART(void)
{
    /* Use PortB, FastIRC clock with Div1 configurations for UART clock */
    HAL_LPUART0_ClockConfig(PortB, FastIRC, Div1);

    /* Disable Receive and Transmit of UART for configurations*/
    HAL_LPUART0_ReceiveConfig(Disable);
    HAL_LPUART0_TransmitConfig(Disable);

    /* Configure number of data bits and stop bits */
    HAL_LPUART0_BitsConfig(EightBits, OneBit);

    /* Disable parity check */
    HAL_LPUART0_ParityConfig(ParDisable);

    /* Configure transmit and receive data to LSB first and disabled inversion */
    HAL_LPUART0_DataConfig(LSBFirst, Disable, Disable);

    /* Set baudrate to 115200 */
    HAL_LPUART0_BaudRateConfig(115200);

    /* Enable receive and transmit through UART */
    HAL_LPUART0_ReceiveConfig(Enable);
    HAL_LPUART0_TransmitConfig(Enable);
}

static void Bootloader_Delay(uint32_t milliseconds)
{
    uint32_t index1;
    uint32_t index2;
    uint32_t count = ((SystemCoreClock / 1000) - 6) / 5;

    for (index1 = 0; index1 < milliseconds; index1++)
    {
        for (index2 = 0; index2 < count; index2++)
        {
            __asm("nop");
        }
    }
}

static Bootloader_Error_Type_t Bootloader_RunBoot(void)
{
    SREC_Error_Type_t errorType = NoErr;
    SREC_Data_Struct_t parsedData;
    Bootloader_Error_Type_t bootError = NoError;

    /* Erase all data in User Application section */
    __disable_irq();
    Erase_Multi_Sector(APP_START_ADDRESS, 32);
    __enable_irq();
    /* Disable transmit interrupt and Enable receive interrupt */
    HAL_LPUART0_InterruptConfig(Disable, Enable);
    /* Continue wait for S-record lines until End Record type is reached or an error occurred*/
    while (SREC_CheckRecType() != EndRec && errorType == NoErr)
    {
        /* Start processing if the queue is not empty */
        while (Queue_CheckQueue() != 0)
        {
            /* Turn on Green and Red LEDs (Yellow) to indicate flashing is in process */
            HAL_GPIO_SetPin(Blue);
            HAL_GPIO_ClearPin(Red);
            HAL_GPIO_ClearPin(Green);
            /* S-record line parsing and error checking */
            errorType = SREC_CheckError(Queue_Pop());
            if (errorType != NoErr)
            {
                bootError = Error;
            }
            /* Write to flash if there is no error with S-record line and it is Data Record type */
            else if (SREC_CheckRecType() == DataRec)
            {
                parsedData = SREC_ObtainData();
                Bootloader_Flash(parsedData.address, parsedData.data, parsedData.dataByteCount);
                HAL_LPUART0_TransmitString(">>", 1);
            }
            else
            {
            }
        }
    }

    return bootError;
}

static void Bootloader_RunApp(void)
{
    /* jumpAddress contain address points to Reset Handler function in User Application */
    uint32_t jumpAddress = *((volatile uint32_t *)(APP_START_ADDRESS + 4));
    /* Declare a function pointer that points to Reset Handler function */
    pFunction jumptoApp = (pFunction)jumpAddress;
    /* Disable all interrupts */
    __disable_irq();
    __DMB();
    /* Write the User Application's Vector Table address to VTOR register */
    SCB->VTOR = APP_START_ADDRESS;
    __DSB();
    /* Set Main (Top) Stack Pointer to the start of User Application section */
    __set_MSP(*((volatile uint32_t *)APP_START_ADDRESS));
    /* Call Reset Handler function of User Application */
    jumptoApp();
}

static void Bootloader_Flash(uint32_t address, uint8_t data[], uint32_t dataByteCount)
{
    uint32_t index = 0;
    for (index = 0; index < dataByteCount; index = index + 4)
    {
        __disable_irq();
        Program_LongWord_8B(address + index, &data[index]);
        __enable_irq();
    }
    for (index = 0; index < dataByteCount; index++)
    {
        data[index] = 0xFF;
    }
}

void PORTBCD_IRQHandler(void)
{
    /* If the button is pressed, switch to Boot mode */
    HAL_GPIO_ClearISF(Switch2);
    sg_Mode = Boot;
}

void LPUART0_IRQHandler(void)
{
    static uint32_t s_lengthcount = 0;
    uint8_t temp = 0;

    temp = HAL_LPUART0_ReceiveCharacter();
    if (temp == '\n' || temp == '\0')
    {
    }
    else
    {
        sg_Buffer[s_lengthcount] = temp;
        if (temp == '\r') /* Push line buffer into queue when End of Line character reached*/
        {
            Queue_Push(sg_Buffer);
            s_lengthcount = 0; /* Reset line length count */
        }
        else
        {
            s_lengthcount++;
        }
    }
}