#ifndef MUTEXRECURSIVE_HPP_
#define MUTEXRECURSIVE_HPP_
/****************************************************************************/
/*  Header    : Recursive Mutex Class										*/
/****************************************************************************/
/*                                                                          */
/*  @file     : MutexRecursive.hpp											*/
/*                                                                          */
/*  @brief	  : FreeRTOS-Recursive-Mutex Wrapper class						*/
/*                                                                          */
/*  @author   : N. Zoller (NZ)                                              */
/*                                                                          */
/*  @date	  : 21.04.2023  NZ	Created                              		*/
/*                                                                          */
/*	@remark	  : Last Modifications:											*/
/* 				- none														*/
/*																			*/
/*  @todo	  :	- Test the whole class extensively							*/
/*																			*/
/****************************************************************************/
/*																			*/
/****************************************************************************/

/* imports */
#include <semphr.h>

/* Class constant declaration  */

/* Class Type declaration      */

/* Class data declaration      */

/* Class definition            */
class MutexRecursive : public Mutex
{
public:
	MutexRecursive(void): Mutex() {};

	~MutexRecursive(void);

	bool takeRecursive(TickType_t ticksToWait);
	bool takeRecursive(void);

	bool giveRecursive(void);
};

/**
 * @brief		Obtain a recursive mutex
 *
 * @param		ticksToWait			Ticks to wait for the semaphore to become available
 * return		True if it was successful, false otherwise
 *
 * @details		Method to recursively obtain, or 'take', a mutex type semaphore.
 * 				The mutex must have previously been created using a call to
 * 				SemaphoreCreateRecursiveMutex().
 * @see			https://www.freertos.org/xSemaphoreTakeRecursive.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline bool MutexRecursive::takeRecursive(TickType_t ticksToWait)
{
	return (xSemaphoreTakeRecursive(handle, ticksToWait) == pdTRUE) ? true : false;
}

/**
 * @brief		Obtain a recursive mutex with default block time
 *
 * @param		void
 * return		True if it was successful, false otherwise
 *
 * @details		Method to recursively obtain, or 'take', a mutex type semaphore.
 * 				The mutex must have previously been created using a call to
 * 				SemaphoreCreateRecursiveMutex(). Sets the ticksToWait to the
 * 				defaultBlockTime. By default this is 0.
 * @see			https://www.freertos.org/xSemaphoreTakeRecursive.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline bool MutexRecursive::takeRecursive(void)
{
	return MutexRecursive::takeRecursive(defaultBlockTime);
}

/**
 * @brief		Release a recursive mutex
 *
 * @param		void
 * return		True if it was successful, false otherwise
 *
 * @details		Method to recursively release, or 'give', a mutex type
 * 				semaphore. The mutex must have previously been created using
 * 				a call to xSemaphoreCreateRecursiveMutex().
 * @see			https://www.freertos.org/xSemaphoreGiveRecursive.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline bool MutexRecursive::giveRecursive(void)
{
	return (xSemaphoreGiveRecursive(handle) == pdTRUE) ? true : false;
}
#endif /* MUTEXRECURSIVE_HPP_ */
