/**
 * @file SREC.h
 * @author William Epic
 * @brief S-record line parsing
 * @version 0.1
 * @date 2022-11-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef INC_SREC_H_
#define INC_SREC_H_

/******************************************************************************* * Definitions ******************************************************************************/
/* This enumeration is to define Error types of a S-record file */
typedef enum
{
    NoErr = 0U,
    RecErr = 1U,
    LineCountErr = 2U,
    ByteCountErr = 3U,
    DigitErr = 4U,
    ChecksumErr = 5U
} SREC_Error_Type_t;

/* This enumeration is to define Reord Types of S-record */
typedef enum
{
    StartRec = 0U,
    DataRec = 1U,
    CountRec = 2U,
    EndRec = 3U
} SREC_RecordType_Type_t;

/* This struct is to store parsed data of a line in S-record file */
typedef struct
{
    uint32_t address;
    uint8_t data[80];
    uint32_t dataByteCount;
} SREC_Data_Struct_t;

/******************************************************************************* * API ******************************************************************************/
/**
 * @brief This function is to check a line of a SREC file for errors
 *
 * @param line this parameter is used to pass an ASCII line of a SREC file
 * @return Error type
 */
SREC_Error_Type_t SREC_CheckError(uint8_t line[]);

/**
 * @brief This function is to check for record type of SREC line
 *
 * @return Record Type
 */
SREC_RecordType_Type_t SREC_CheckRecType(void);

/**
 * @brief This function is to obtain parsed and checked data in a SREC line
 *
 * @return Parsed data (address, data and data length)
 */
SREC_Data_Struct_t SREC_ObtainData(void);

#endif /* INC_SREC_H_ */