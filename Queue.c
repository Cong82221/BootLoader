#include <stdint.h>
#include "Queue.h"

/******************************************************************************* * Definitions ******************************************************************************/
#define LIMIT_TOTAL_RECORD_LENGTH 80
#define NUMBER_OF_QUEUE_ELEMENTS 4

/* This struct is to store contents of a queue element */
typedef struct
{
    uint8_t payload[LIMIT_TOTAL_RECORD_LENGTH];
} Queue_Content_Struct_t;

/******************************************************************************* * Prototypes ******************************************************************************/

/******************************************************************************* * Variables ******************************************************************************/
static Queue_Content_Struct_t sg_Queue[NUMBER_OF_QUEUE_ELEMENTS]; /* Queue array with 4 elements */
static uint8_t sg_NumberofElements = 0;                           /* Count number of elements when push and pop */

/******************************************************************************* * Code ******************************************************************************/
void Queue_Push(uint8_t line[])
{
    static uint8_t s_pushIndex = 0;
    uint32_t lineIndex = 0;

    while (line[lineIndex] != '\r')
    {
        sg_Queue[s_pushIndex].payload[lineIndex] = line[lineIndex];
        lineIndex++;
    }
    for (; lineIndex < LIMIT_TOTAL_RECORD_LENGTH; lineIndex++)
    {
        sg_Queue[s_pushIndex].payload[lineIndex] = '\0'; /* Write all excess slots of the queue element to '\0' */
    }
    s_pushIndex++;
    if (s_pushIndex >= NUMBER_OF_QUEUE_ELEMENTS)
    {
        s_pushIndex = 0; /* Move push index to the start of the queue when the end of queue is reached */
    }
    sg_NumberofElements++;
}

uint8_t *Queue_Pop(void)
{
    static uint8_t s_popIndex = 0;
    uint8_t *p_payloadPointer;

    p_payloadPointer = sg_Queue[s_popIndex].payload; /* Stores the address of the payload of the queue element */
    s_popIndex++;
    if (s_popIndex >= NUMBER_OF_QUEUE_ELEMENTS)
    {
        s_popIndex = 0; /* Move pop index to the start of the queue when the end of queue is reached */
    }
    sg_NumberofElements--;

    return p_payloadPointer;
}

uint8_t Queue_CheckQueue(void)
{
    return sg_NumberofElements;
}