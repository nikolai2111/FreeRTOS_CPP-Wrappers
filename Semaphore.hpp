#ifndef SEMAPHORE_HPP_
#define SEMAPHORE_HPP_
/****************************************************************************/
/*  Header    : Semaphore Class												*/
/****************************************************************************/
/*                                                                          */
/*  @file     : Semaphore.hpp												*/
/*                                                                          */
/*  @brief	  : FreeRTOS-Semaphore Wrapper class							*/
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
class Semaphore : public FreeRTOS
{
protected:
	SemaphoreHandle_t		handle;
	const UBaseType_t 		maxCount;

	TickType_t				defaultBlockTime = 0;

public:
	Semaphore(void);
	Semaphore(UBaseType_t maxCount, UBaseType_t initialCount);

	~Semaphore(void);

	TaskHandle_t getHolder(void);

	UBaseType_t getCount(void);

	bool take(TickType_t ticksToWait);
	bool take(void);

	bool takeFromISR(void);

	bool give(void);

	bool giveFromISR(void);
};

/**
 * @brief		Constructor binary semaphore
 *
 * @param		void
 *
 * @details		Constructs a new binary semaphore object with the FreeRTOS API
 * 				function xSemaphoreCreateBinary().
 * @see			https://www.freertos.org/xSemaphoreCreateBinary.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline Semaphore::Semaphore(void): maxCount(1)
{
	handle = xSemaphoreCreateBinary();

	assert(handle != NULL);

	give();
};

/**
 * @brief		Constructor counting semaphore
 *
 * @param		maxSemphrCount		The maximum count value that can be reached
 * @param		initialCount		The value assigned to the semaphore when it is created
 *
 * @details		Constructs a new counting semaphore object with the FreeRTOS
 * 				API function xSemaphoreCreateCounting().
 * @see			https://www.freertos.org/CreateCounting.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline Semaphore::Semaphore(UBaseType_t maxSemphrCount, UBaseType_t initialCount): maxCount(maxSemphrCount)
{
	handle = xSemaphoreCreateCounting(maxCount, initialCount);

	assert(handle != NULL);

	give();
};

/**
 * @brief		Destructor
 *
 * @param		void
 *
 * @details		Deletes a timer object with the FreeRTOS API function
 * 				vSemaphoreDelete()
 * @see			https://www.freertos.org/CreateCounting.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline Semaphore::~Semaphore(void)
{
	vSemaphoreDelete(handle);
};

/**
 * @brief		Gets the handler of holder task
 *
 * @param		void
 * return		Return the handle of the task that holds the mutex
 *
 * @details		Can be used reliably to determine if the calling task is the
 * 				mutex holder, but cannot be used reliably if the mutex is held
 * 				by any task other than the calling task.
 * @see			https://www.freertos.org/xSemaphoreGetMutexHolder.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline TaskHandle_t Semaphore::getHolder(void)
{
	return xSemaphoreGetMutexHolder(handle);
}

/**
 * @brief		Gets the count of the semaphore
 *
 * @param		void
 * return		1 is returned if the semaphore is available, and 0 if not available
 *
 * @details		If the semaphore is a counting semaphore then the semaphores
 * 				current count value is returned. If the semaphore is a binary
 * 				semaphore then 1 is returned if the semaphore is available,
 * 				and 0 is returned if the semaphore is not available.
 * @see			https://www.freertos.org/uxSemaphoreGetCount.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline UBaseType_t Semaphore::getCount(void)
{
	return uxSemaphoreGetCount(handle);
}

/**
 * @brief		Obtain a semaphore
 *
 * @param		ticksToWait			Ticks to wait for the semaphore to become available
 * return		True if it was successful, false otherwise
 *
 * @details		Method to obtain a semaphore. The semaphore must have previously
 * 				been created with a call to xSemaphoreCreateBinary(),
 * 				xSemaphoreCreateMutex() or xSemaphoreCreateCounting().
 * @see			https://www.freertos.org/a00122.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline bool Semaphore::take(TickType_t ticksToWait)
{
	return (xSemaphoreTake(handle, ticksToWait) == pdTRUE) ? true : false;
}

/**
 * @brief		Obtain a semaphore with default block time
 *
 * @param		void
 * return		True if it was successful, false otherwise
 *
 * @details		Method to obtain a semaphore. The semaphore must have previously
 * 				been created with a call to xSemaphoreCreateBinary(),
 * 				xSemaphoreCreateMutex() or xSemaphoreCreateCounting(). Sets the
 * 				ticksToWait to the defaultBlockTime. By default this is 0.
 * @see			https://www.freertos.org/a00122.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline bool Semaphore::take(void)
{
	return Semaphore::take(defaultBlockTime);
}

/**
 * @brief		Obtain a semaphore from ISR
 *
 * @param		void
 * return		True if it was successful, false otherwise
 *
 * @details		Method to obtain a semaphore from an interrupt service routine.
 * 				The semaphore must have previously been created with a call to
 * 				xSemaphoreCreateBinary(), xSemaphoreCreateMutex() or
 * 				xSemaphoreCreateCounting().
 * @see			https://www.freertos.org/xSemaphoreTakeFromISR.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline bool Semaphore::takeFromISR(void)
{
	return (xSemaphoreTakeFromISR(handle, NULL) == pdTRUE) ? true : false;
}

/**
 * @brief		Release a semaphore
 *
 * @param		void
 * return		True if it was successful, false otherwise
 *
 * @details		Method to release a semaphore. The semaphore must have previously
 * 				been created with a call to xSemaphoreCreateBinary(),
 * 				xSemaphoreCreateMutex() or xSemaphoreCreateCounting().
 * @see			https://www.freertos.org/a00123.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline bool Semaphore::give(void)
{
	return (xSemaphoreGive(handle) == pdTRUE) ? true : false;
}

/**
 * @brief		Release a semaphore from ISR
 *
 * @param		void
 * return		True if it was successful, false otherwise
 *
 * @details		Method to release a semaphore from an interrupt service routine.
 * 				The semaphore must have previously been created with a call to
 * 				xSemaphoreCreateBinary(), xSemaphoreCreateMutex() or
 * 				xSemaphoreCreateCounting().
 * @see			https://www.freertos.org/a00124.html
 *
 * @author		N. Zoller (NZ)
 * @date		21.04.2023	NZ	Created
 ****************************************************************************/
inline bool Semaphore::giveFromISR(void)
{
	return (xSemaphoreGiveFromISR(handle, NULL) == pdTRUE) ? true : false;
}
#endif /* SEMAPHORE_HPP_ */
