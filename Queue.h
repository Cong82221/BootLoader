/**
 * @file Queue.h
 * @author William Epic
 * @brief Queue manipulations
 * @version 0.1
 * @date 2022-11-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

/******************************************************************************* * Definitions ******************************************************************************/

/******************************************************************************* * API ******************************************************************************/
/**
 * @brief This function is to push an element to store in the queue
 *
 * @param line this parameter is used to pass a line into the queue
 */
void Queue_Push(uint8_t line[]);

/**
 * @brief This funtion is to pop an element out of the queue
 *
 * @return The address of an array stored in queue
 */
uint8_t *Queue_Pop(void);

/**
 * @brief This function is to output the number of elements currently in the queue
 *
 * @return Number of elements currently in queue
 */
uint8_t Queue_CheckQueue(void);

#endif /* INC_QUEUE_H_ */