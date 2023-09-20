#include "MKE16Z4.h"
#include "HAL_LPUART0.h"

/******************************************************************************* * Definitions ******************************************************************************/
/* This enumeration is to define Alternatives of each pin of a port */
typedef enum
{
    Alt0 = 0U,
    Alt1 = 1U,
    Alt2 = 2U,
    Alt3 = 3U,
    Alt4 = 4U,
    Alt5 = 5U,
    Alt6 = 6U,
    Alt7 = 7U
} HAL_LPUART0_PinAlternative_Type_t;

/******************************************************************************* * Prototypes ******************************************************************************/

/******************************************************************************* * Variables ******************************************************************************/
static uint32_t sg_LPUART0_Clock = 0;

/******************************************************************************* * Code ******************************************************************************/
void HAL_LPUART0_ClockConfig(HAL_LPUART0_Port_Type_t portSelect, HAL_LPUART0_ClockName_Type_t clockName, HAL_LPUART0_ClockDiv_Type_t clockDiv)
{
    uint32_t clockValue;

    switch (clockName)
    {
    case SysOSC:
        SCG->SOSCDIV = (SCG->SOSCDIV & ~SCG_SOSCDIV_SOSCDIV2_MASK) | SCG_SOSCDIV_SOSCDIV2(clockDiv);
        clockValue = CPU_XTAL_CLK_HZ / clockDiv;
        break;
    case SlowIRC:
        SCG->SIRCDIV = (SCG->SIRCDIV & ~SCG_SIRCDIV_SIRCDIV2_MASK) | SCG_SIRCDIV_SIRCDIV2(clockDiv);
        clockValue = CPU_INT_SLOW_CLK_HZ / clockDiv;
        break;
    case FastIRC:
        SCG->FIRCDIV = (SCG->FIRCDIV & ~SCG_FIRCDIV_FIRCDIV2_MASK) | SCG_FIRCDIV_FIRCDIV2(clockDiv);
        clockValue = CPU_INT_FAST_CLK_HZ / clockDiv;
        break;
    case LPFLL:
        SCG->LPFLLDIV = (SCG->LPFLLDIV & ~SCG_LPFLLDIV_LPFLLDIV2_MASK) | SCG_LPFLLDIV_LPFLLDIV2(clockDiv);
        clockValue = (48000000U + ((SCG->LPFLLCFG & SCG_LPFLLCFG_FSEL_MASK) >> SCG_LPFLLCFG_FSEL_SHIFT) * 24000000U) / clockDiv;
        break;
    default:
        break;
    }
    switch (portSelect)
    {
    case PortA:
        PCC->CLKCFG[PCC_PORTA_INDEX] = (PCC->CLKCFG[PCC_PORTA_INDEX] & ~PCC_CLKCFG_CGC_MASK) | PCC_CLKCFG_CGC(Enable);
        PORTA->PCR[2] = (PORTA->PCR[2] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(Alt6);
        PORTA->PCR[3] = (PORTA->PCR[3] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(Alt6);
        PCC->CLKCFG[PCC_LPUART0_INDEX] = (PCC->CLKCFG[PCC_LPUART0_INDEX] & ~PCC_CLKCFG_PCS_MASK) | PCC_CLKCFG_PCS(clockName);
        PCC->CLKCFG[PCC_LPUART0_INDEX] = (PCC->CLKCFG[PCC_LPUART0_INDEX] & ~PCC_CLKCFG_CGC_MASK) | PCC_CLKCFG_CGC(Enable);
        break;

    case PortB:
        PCC->CLKCFG[PCC_PORTB_INDEX] = (PCC->CLKCFG[PCC_PORTB_INDEX] & ~PCC_CLKCFG_CGC_MASK) | PCC_CLKCFG_CGC(Enable);
        PORTB->PCR[0] = (PORTB->PCR[0] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(Alt2);
        PORTB->PCR[1] = (PORTB->PCR[1] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(Alt2);
        PCC->CLKCFG[PCC_LPUART0_INDEX] = (PCC->CLKCFG[PCC_LPUART0_INDEX] & ~PCC_CLKCFG_PCS_MASK) | PCC_CLKCFG_PCS(clockName);
        PCC->CLKCFG[PCC_LPUART0_INDEX] = (PCC->CLKCFG[PCC_LPUART0_INDEX] & ~PCC_CLKCFG_CGC_MASK) | PCC_CLKCFG_CGC(Enable);
        break;

    default:
        break;
    }
    sg_LPUART0_Clock = clockValue;
}

void HAL_LPUART0_ReceiveConfig(HAL_LPUART0_Status_Type_t status)
{
    LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_RE_MASK) | LPUART_CTRL_RE(status);
}

void HAL_LPUART0_TransmitConfig(HAL_LPUART0_Status_Type_t status)
{
    LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_TE_MASK) | LPUART_CTRL_TE(status);
}

void HAL_LPUART0_BitsConfig(HAL_LPUART0_NumberofBits_Type_t numberofDataBits, HAL_LPUART0_NumberofBits_Type_t numberofStopBits)
{
    switch (numberofDataBits)
    {
    case SevenBits:
        LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_M_MASK) | LPUART_CTRL_M(0);
        LPUART0->BAUD = (LPUART0->BAUD & ~LPUART_BAUD_M10_MASK) | LPUART_BAUD_M10(0);
        LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_M7_MASK) | LPUART_CTRL_M7(1);
        break;

    case EightBits:
        LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_M7_MASK) | LPUART_CTRL_M7(0);
        LPUART0->BAUD = (LPUART0->BAUD & ~LPUART_BAUD_M10_MASK) | LPUART_BAUD_M10(0);
        LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_M_MASK) | LPUART_CTRL_M(0);
        break;

    case NineBits:
        LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_M7_MASK) | LPUART_CTRL_M7(0);
        LPUART0->BAUD = (LPUART0->BAUD & ~LPUART_BAUD_M10_MASK) | LPUART_BAUD_M10(0);
        LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_M_MASK) | LPUART_CTRL_M(1);
        break;

    case TenBits:
        LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_M7_MASK) | LPUART_CTRL_M7(0);
        LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_M_MASK) | LPUART_CTRL_M(0);
        LPUART0->BAUD = (LPUART0->BAUD & ~LPUART_BAUD_M10_MASK) | LPUART_BAUD_M10(1);
        break;

    default:
        break;
    }

    switch (numberofStopBits)
    {
    case OneBit:
        LPUART0->BAUD = (LPUART0->BAUD & ~LPUART_BAUD_SBNS_MASK) | LPUART_BAUD_SBNS(0);
        break;

    case TwoBits:
        LPUART0->BAUD = (LPUART0->BAUD & ~LPUART_BAUD_SBNS_MASK) | LPUART_BAUD_SBNS(1);
        break;

    default:
        break;
    }
}

void HAL_LPUART0_ParityConfig(HAL_LPUART0_Parity_Type_t parityType)
{
    if (parityType == ParDisable)
    {
        LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_PE_MASK) | LPUART_CTRL_PE(Disable);
    }
    else
    {
        LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_PE_MASK) | LPUART_CTRL_PE(Enable);
        LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_PT_MASK) | LPUART_CTRL_PT(parityType);
    }
}

void HAL_LPUART0_DataConfig(HAL_LPUART0_MSBF_Type_t msbf, HAL_LPUART0_Status_Type_t rxinvEn, HAL_LPUART0_Status_Type_t txinvEn)
{
    LPUART0->STAT = (LPUART0->STAT & ~LPUART_STAT_MSBF_MASK) | LPUART_STAT_MSBF(msbf);
    LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_TXINV_MASK) | LPUART_CTRL_TXINV(txinvEn);
    LPUART0->STAT = (LPUART0->STAT & ~LPUART_STAT_RXINV_MASK) | LPUART_STAT_RXINV(rxinvEn);
}

void HAL_LPUART0_BaudRateConfig(uint32_t baudrate)
{
    uint8_t index = 0;
    uint8_t osr = 0;
    uint32_t sbr = 0;
    uint32_t tempsbr = 0;
    uint32_t tempbaud = 0;
    uint32_t diff = 0;
    uint32_t mindiff = 0;

    osr = 3;
    tempsbr = sg_LPUART0_Clock / (baudrate * (osr + 1));
    tempbaud = sg_LPUART0_Clock / (tempsbr * (osr + 1));
    mindiff = baudrate - tempbaud;
    for (index = 4; index < 32; index++)
    {
        tempsbr = sg_LPUART0_Clock / (baudrate * (index + 1));
        tempbaud = sg_LPUART0_Clock / (tempsbr * (index + 1));
        if (tempbaud > baudrate)
        {
            diff = tempbaud - baudrate;
        }
        else
        {
            diff = baudrate - tempbaud;
        }
        if (diff < mindiff)
        {
            mindiff = diff;
            osr = index;
            sbr = tempsbr;
        }
    }
    if (osr >= 3 || osr <= 6)
    {
        LPUART0->BAUD = (LPUART0->BAUD & ~LPUART_BAUD_BOTHEDGE_MASK) | LPUART_BAUD_BOTHEDGE(Enable);
    }
    else
    {
        LPUART0->BAUD = (LPUART0->BAUD & ~LPUART_BAUD_BOTHEDGE_MASK) | LPUART_BAUD_BOTHEDGE(Disable);
    }
    LPUART0->BAUD = (LPUART0->BAUD & ~LPUART_BAUD_SBR_MASK) | LPUART_BAUD_SBR(sbr);
    LPUART0->BAUD = (LPUART0->BAUD & ~LPUART_BAUD_OSR_MASK) | LPUART_BAUD_OSR(osr);
}

void HAL_LPUART0_InterruptConfig(HAL_LPUART0_Status_Type_t txIRQ, HAL_LPUART0_Status_Type_t rxIRQ)
{
    if (txIRQ == Enable || rxIRQ == Enable)
    {
        NVIC_EnableIRQ(LPUART0_IRQn);
    }
    else
    {
        NVIC_DisableIRQ(LPUART0_IRQn);
    }
    LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_TIE_MASK) | LPUART_CTRL_TIE(txIRQ);
    LPUART0->CTRL = (LPUART0->CTRL & ~LPUART_CTRL_RIE_MASK) | LPUART_CTRL_RIE(rxIRQ);
}

void HAL_LPUART0_TransmitCharacter(uint8_t character)
{
    while ((LPUART0->STAT & LPUART_STAT_TDRE_MASK) >> LPUART_STAT_TDRE_SHIFT == 0)
    {
    }
    LPUART0->DATA = (LPUART0->DATA & (~0xFF)) | (uint32_t)character;
}

void HAL_LPUART0_TransmitString(uint8_t *string, uint32_t length)
{
    uint32_t index = 0;

    for (index = 0; index < length; index++)
    {
        HAL_LPUART0_TransmitCharacter(*(string + index));
    }
}

uint8_t HAL_LPUART0_ReceiveCharacter(void)
{
    return (uint8_t)LPUART0->DATA & 0xFF;
}