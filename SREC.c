#include <stdint.h>
#include "SREC.h"

/******************************************************************************* * Definitions ******************************************************************************/
#define MAX_HEX_STRING_ELEMENTS 50                  /* Maximum Hex numbers in a line */

/******************************************************************************* * Prototypes ******************************************************************************/
/**
 * @brief This function is to count the number of ASCII characters of a line
 *
 * @param line this parameter is to pass the line into the function
 * @return Line count
 */
static uint32_t SREC_LengthCount(uint8_t line[]);

/**
 * @brief This function is to conver ASCII digits into Hex Number and store it
 *
 * @param firstDigit this parameter is the first ASCII digit of a Hex Number
 * @param secondDigit this parameter is the second ASCII digit of a Hex Number
 * @param storeHexValue this parameter is to store the converted Hex Number
 * @return Error code
 */
static SREC_Error_Type_t SREC_AsciitoHex(uint8_t firstDigit, uint8_t secondDigit, uint8_t *storeHexValue);

/******************************************************************************* * Variables ******************************************************************************/
static uint8_t sg_HexLine[MAX_HEX_STRING_ELEMENTS];  /* Array to store converted Hex numbers from ASCII line */
static uint32_t sg_HexCount = 0;                     /* Number of Hex numbers in the line (including S and Record Type) */
static SREC_RecordType_Type_t sg_RecType = StartRec; /* Record type of parsing line */

/******************************************************************************* * Code ******************************************************************************/
SREC_Error_Type_t SREC_CheckError(uint8_t line[])
{
    SREC_Error_Type_t errorType = NoErr;
    uint32_t index = 0;
    uint32_t byteCount = 0;
    uint32_t lineCount = 0;
    uint8_t sum = 0;
    static uint32_t s_dataLineCount = 0;

    sg_HexCount = 0;
    /* Check Record Start and Record Type */
    if (line[0] != 'S' || line[1] > '9' || line[1] < '0')
    {
        errorType = RecErr;
    }
    else
    {
        sg_HexLine[0] = line[0];
        sg_HexLine[1] = line[1] - '0';
        sg_HexCount = 2;
        /* Convert 2 ASCII characters to 1 Hex value and check Hex Digit Error */
        for (index = 2; index < SREC_LengthCount(line); index = index + 2)
        {
            errorType = SREC_AsciitoHex(line[index], line[index + 1], &sg_HexLine[sg_HexCount]);
            if (errorType == DigitErr)
            {
                index = SREC_LengthCount(line);
            }
            else
            {
                sg_HexCount++;
            }
        }
        if (sg_HexLine[1] == 0)
        {
            sg_RecType = StartRec;
        }
        /* Accumulate Data line count for each line with record type 1, 2 or 3 */
        if (sg_HexLine[1] == 1 || sg_HexLine[1] == 2 || sg_HexLine[1] == 3)
        {
            s_dataLineCount++;
            sg_RecType = DataRec;
        }
        /* Check if number of Data lines count matches with value of lines with record type 5 or 6 */
        if (sg_HexLine[1] == 5 || sg_HexLine[1] == 6)
        {
            sg_RecType = CountRec;
            switch (sg_HexLine[1])
            {
            case 5:
                lineCount = (sg_HexLine[2] << 8) | sg_HexLine[3]; /* 16 bit value */
                break;
            case 6:
                lineCount = (sg_HexLine[2] << 16) | (sg_HexLine[3] << 8) | sg_HexLine[4]; /* 24 bit value */
                break;
            default:
                break;
            }
            if (lineCount != s_dataLineCount)
            {
                errorType = LineCountErr;
            }
        }
        if (sg_HexLine[1] == 7 || sg_HexLine[1] == 8 || sg_HexLine[1] == 9)
        {
            sg_RecType = EndRec;
        }
    }
    if (errorType == NoErr)
    {
        /* Check byte count */
        byteCount = sg_HexLine[2];
        if ((sg_HexCount - 3) != byteCount)
        {
            errorType = ByteCountErr;
        }
        /* Check Checksum value */
        else
        {
            for (index = 2; index < sg_HexCount; index++)
            {
                sum = sum + sg_HexLine[index];
            }
            if (sum != 0xFF)
            {
                errorType = ChecksumErr;
            }
            else
            {
                errorType = NoErr;
            }
        }
    }

    return errorType;
}

SREC_Data_Struct_t SREC_ObtainData(void)
{
    SREC_Data_Struct_t lineData;
    uint32_t index = 0;
    uint32_t dataCount = 0;

    switch (sg_HexLine[1])
    {
    case 1:
        lineData.address = (uint32_t)((sg_HexLine[3] << 8) | sg_HexLine[4]);
        for (index = 0; index < sg_HexCount - 6; index++)
        {
            lineData.data[dataCount] = sg_HexLine[index + 5];
            dataCount++;
        }
        lineData.dataByteCount = dataCount;
        break;

    case 2:
        lineData.address = (uint32_t)((sg_HexLine[3] << 16) | (sg_HexLine[4] << 8) | sg_HexLine[5]);
        for (index = 0; index < sg_HexCount - 7; index++)
        {
            lineData.data[dataCount] = sg_HexLine[index + 6];
            dataCount++;
        }
        lineData.dataByteCount = dataCount;
        break;

    case 3:
        lineData.address = (uint32_t)((sg_HexLine[3] << 24) | (sg_HexLine[4] << 16) | (sg_HexLine[5] << 8) | sg_HexLine[6]);
        for (index = 0; index < sg_HexCount - 8; index++)
        {
            lineData.data[dataCount] = sg_HexLine[index + 7];
            dataCount++;
        }
        lineData.dataByteCount = dataCount;
        break;

    default:
        break;
    }

    return lineData;
}

SREC_RecordType_Type_t SREC_CheckRecType(void)
{
    return sg_RecType;
}

static uint32_t SREC_LengthCount(uint8_t line[])
{
    uint32_t count = 0;

    while (line[count] != '\0')
    {
        count++;
    }

    return count;
}

static SREC_Error_Type_t SREC_AsciitoHex(uint8_t firstDigit, uint8_t secondDigit, uint8_t *storeHexValue)
{
    SREC_Error_Type_t error = NoErr;

    /* Convert first digit to decimal */
    if (firstDigit >= '0' && firstDigit <= '9')
    {
        firstDigit = firstDigit - '0';
        error = NoErr;
    }
    else if (firstDigit >= 'A' && firstDigit <= 'F')
    {
        firstDigit = firstDigit - '7';
        error = NoErr;
    }
    else
    {
        error = DigitErr;
    }
    /* Convert second digit to decimal */
    if (error != DigitErr)
    {
        if (secondDigit >= '0' && secondDigit <= '9')
        {
            secondDigit = secondDigit - '0';
            error = NoErr;
        }
        else if (secondDigit >= 'A' && secondDigit <= 'F')
        {
            secondDigit = secondDigit - '7';
            error = NoErr;
        }
        else
        {
            error = DigitErr;
        }
    }
    /* Convert 2 digits to Hexadecimal */
    *storeHexValue = (firstDigit << 4) | secondDigit;

    return error;
}