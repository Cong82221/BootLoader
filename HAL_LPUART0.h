/**
 * @file HAL_LPUART0.h
 * @author William Epic
 * @brief Low-power Universal Asynchronous Receiver-Transmitter (LPUART0) Configurations
 * @version 0.1
 * @date 2022-11-07
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef INC_HAL_LPUART0_H_
#define INC_HAL_LPUART0_H_

/******************************************************************************* * Definitions ******************************************************************************/
/* This enumeration is to define Ports for LPUART0 */
typedef enum
{
    PortA = 0U,
    PortB = 1U,
} HAL_LPUART0_Port_Type_t;

/* This enumeration is to define Clock Source */
typedef enum
{
    SysOSC = 1U,
    SlowIRC = 2U,
    FastIRC = 3U,
    LPFLL = 5U
} HAL_LPUART0_ClockName_Type_t;

/* This enumeration is to define Modes for dividers */
typedef enum
{
    Div1 = 1U,
    Div2 = 2U,
    Div4 = 4U,
    Div8 = 8U,
    Div16 = 16U,
    Div32 = 32U,
    Div64 = 64U
} HAL_LPUART0_ClockDiv_Type_t;

/* This enumeration is to define Status types */
typedef enum
{
    Disable = 0U,
    Enable = 1U
} HAL_LPUART0_Status_Type_t;

/* This enumeration is to define Parity types */
typedef enum
{
    ParDisable = 0U,
    Even = 1U,
    Odd = 2U
} HAL_LPUART0_Parity_Type_t;

/* This enumeration is to define MSB or LSB first */
typedef enum
{
    LSBFirst = 0U,
    MSBFirst = 1U
} HAL_LPUART0_MSBF_Type_t;

/* This enumeration is to define Number of bits for data bits and stop bit configuration */
typedef enum
{
    OneBit = 0U,
    TwoBits = 1U,
    SevenBits = 7U,
    EightBits = 8U,
    NineBits = 9U,
    TenBits = 10U
} HAL_LPUART0_NumberofBits_Type_t;

/******************************************************************************* * API ******************************************************************************/

/**
 * @brief This function is to configure clock for LPUART0 module
 *
 * @param portSelect is used to select which port LPUART0 is directed to
 * @param clockName is used to determine clock source for LPUART0
 * @param clockDiv is used to determine the divide parameter of clock dividers
 */
void HAL_LPUART0_ClockConfig(HAL_LPUART0_Port_Type_t portSelect, HAL_LPUART0_ClockName_Type_t clockName, HAL_LPUART0_ClockDiv_Type_t clockDiv);

/**
 * @brief This function is to configure receive function of LPUART0
 *
 * @param status is used to determine whether to enable or disable receive of LPUART0
 */
void HAL_LPUART0_ReceiveConfig(HAL_LPUART0_Status_Type_t status);

/**
 * @brief This function is to configure transmit function of LPUART0
 *
 * @param status is used to determine whether to enable or disable transmit of LPUART0
 */
void HAL_LPUART0_TransmitConfig(HAL_LPUART0_Status_Type_t status);

/**
 * @brief This function is to configure number of data bits and number of stop bits of LPUART0
 *
 * @param numberofDataBits is used to determine number of data bits
 * @param numberofStopBits is used to determine number of stop bits
 */
void HAL_LPUART0_BitsConfig(HAL_LPUART0_NumberofBits_Type_t numberofDataBits, HAL_LPUART0_NumberofBits_Type_t numberofStopBits);

/**
 * @brief This function is to configure parity check of LPUART0
 *
 * @param parityType is used to determine which parity mode of LPUART0
 */
void HAL_LPUART0_ParityConfig(HAL_LPUART0_Parity_Type_t parityType);

/**
 * @brief This function is to configure transmit and receive data
 *
 * @param msbf is used to determine MSB or LSB to be received or transmitted first
 * @param rxinvEn is used to determine whether to enable or disable receive data inversion of LPUART0
 * @param txinvEn is used to determine whether to enable or disable transmit data inversion of LPUART0
 */
void HAL_LPUART0_DataConfig(HAL_LPUART0_MSBF_Type_t msbf, HAL_LPUART0_Status_Type_t rxinvEn, HAL_LPUART0_Status_Type_t txinvEn);

/**
 * @brief This function is to automatically set baudrate of LPUART0 nearest to the desired baudrate
 *
 * @param baudrate is the desired baudrate
 */
void HAL_LPUART0_BaudRateConfig(uint32_t baudrate);

/**
 * @brief This function is to configure interrupt for LPUART0
 *
 * @param txIRQ is used to determine whether to enable or disable transmit interrupt of LPUART0
 * @param rxIRQ is used to determine whether to enable or disable receive interrupt of LPUART0
 */
void HAL_LPUART0_InterruptConfig(HAL_LPUART0_Status_Type_t txIRQ, HAL_LPUART0_Status_Type_t rxIRQ);

/**
 * @brief This function is to transmit a character through LPUART0
 *
 * @param character is the character desired to transmit
 */
void HAL_LPUART0_TransmitCharacter(uint8_t character);

/**
 * @brief This function is to transmit a string through LPUART0
 *
 * @param string is the string desired to transmit
 * @param length is the length of the string
 */
void HAL_LPUART0_TransmitString(uint8_t *string, uint32_t length);

/**
 * @brief This function is to receive a character through LPUART0
 *
 * @return The received character from LPUART0
 */
uint8_t HAL_LPUART0_ReceiveCharacter(void);

#endif /* INC_HAL_LPUART0_H_ */