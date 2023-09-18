#ifndef QUEUE_HPP_
#define QUEUE_HPP_
/****************************************************************************/
/*  Header    : Queue Class													*/
/****************************************************************************/
/*                                                                          */
/*  @file     : Queue.hpp													*/
/*                                                                          */
/*  @brief	  : FreeRTOS-Queue Wrapper class								*/
/*                                                                          */
/*  @author   : N. Zoller (NZ)                                              */
/*                                                                          */
/*  @date	  : 08.04.2023  NZ	Created                              		*/
/*                                                                          */
/*	@remark	  : Last Modifications:											*/
/* 				- 10.04.2023	NZ	Mod: Header, add doxygen commands		*/
/*				- 17.04.2023	NZ	Mod: Modified class, to a template class*/
/*				- 21.04.2023	NZ	Mod: Combined .cpp and .hpp and made the*/
/*									methods inline							*/
/*                                                                          */
/*  @todo	  :	- Add this functions from API								*/
/*					- xQueueCreateStatic() 									*/
/*					- xQueueOverwrite() 									*/
/*					- xQueueOverwriteFromISR()								*/
/*					- xQueuePeek()		 									*/
/*					- xQueuePeekFromISR()									*/
/*					- xQueueIsQueueFullFromISR()							*/
/*					- xQueueIsQueueEmptyFromISR()							*/
/*				- Test the whole class extensively							*/
/*																			*/
/****************************************************************************/
/*																			*/
/****************************************************************************/

/* imports */
#include <string>

#include <FreeRTOS.h>
#include <queue.h>

/* Class constant declaration  */

/* Class Type declaration      */

/* Class data declaration      */

/* Class definition            */

template <typename T>
class Queue : public FreeRTOS
{
protected:
	QueueHandle_t		handle;
	const UBaseType_t	length;
	const std::string	name = "";

	TickType_t			defaultMaxTicksToWait = portMAX_DELAY;
	TickType_t			defaultMinTicksToWait = 0;

public:
	Queue(UBaseType_t queueLength);
	Queue(UBaseType_t queueLength, bool addToRegistry, std::string queueName);

	~Queue(void);

	bool reset(void);

	bool sendToBack(const T itemToQueue, TickType_t ticksToWait);
	bool sendToBack(const T itemToQueue);

	bool sendToBackFromISR(const T itemToQueue);

	bool sendToFront(const T itemToQueue, TickType_t ticksToWait);
	bool sendToFront(const T itemToQueue);

	bool sendToFrontFromISR(const T itemToQueue);

	T receive(TickType_t ticksToWait);
	T receive(void);

	T receiveFromISR(void);

	UBaseType_t messagesWaiting(void);

	UBaseType_t messagesWaitingFromISR(void);

	UBaseType_t spacesAvailable(void);

	void setDefaultMaxTicksToWait(TickType_t newTicksToWait);

	void setDefaultMinTicksToWait(TickType_t newTicksToWait);
};

/**
 * @brief		Constructor
 *
 * @param		queueLength		Length of the queue (max. items in queue)
 * @param		queueItemSize	Size of one item in the queue
 *
 * @details		Constructs a new queue object with the FreeRTOS API function
 * 				xQueueCreate().
 * @see			https://www.freertos.org/a00116.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline Queue<T>::Queue(UBaseType_t queueLength):length(queueLength)
{
	handle = xQueueCreate(length, sizeof(T));

	assert(handle != NULL);
};

/**
 * @brief		Constructor, with add to registry
 *
 * @param		queueLength		Length of the queue (max. items in queue)
 * @param		queueItemSize	Size of one item in the queue
 *
 * @details		Constructs a new queue object with the FreeRTOS API function
 * 				xQueueCreate() and adds it to the registry for easy debugging.
 * @see			https://www.freertos.org/a00116.html
 * @see			https://www.freertos.org/vQueueAddToRegistry.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline Queue<T>::Queue(UBaseType_t queueLength, bool addToRegistry, std::string queueName):length(queueLength), name(queueName)
{
	handle = xQueueCreate(length, sizeof(T));

	assert(handle != NULL);

	if (addToRegistry == true) {
		vQueueAddToRegistry(handle, (const char * const) name.c_str());
	}
};

/**
 * @brief		Destructor
 *
 * @param		void
 *
 * @details		Deletes a queue object with the FreeRTOS API function
 * 				vQueueDelete().
 * @see			https://www.freertos.org/a00018.html#vQueueDelete
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline Queue<T>::~Queue(void)
{
	vQueueDelete(handle);
};

/**
 * @brief		Resets a queue to its original empty state
 *
 * @param		void
 * @return		True if it was successful, false otherwise
 *
 * @details		Resets a queue to its original empty state like the
 * 				FreeRTOS API function.
 * @warning		Since FreeRTOS V7.2.0 xQueueReset() always returns pdPASS.
 * @see			https://www.freertos.org/a00018.html#xQueueReset
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline bool Queue<T>::reset(void)
{
	return (xQueueReset(handle) == pdPASS) ? true : false;
}

/**
 * @brief		Sends item to queue back and waits the given ticks
 *
 * @param		itemToQueue		Item to push to back
 * @param		ticksToWait		Ticks to wait to complete
 * @return		True if it was successful, false otherwise
 *
 * @details		Sends an item to the back of the queue and waits the given
 * 				amount of time. For the maximum of wait time use the macro
 * 				`portMAX_DELAY` or the function `sendToBack(const void *itemToQueue)`.
 * @see			https://www.freertos.org/xQueueSendToBack.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline bool Queue<T>::sendToBack(const T itemToQueue, TickType_t ticksToWait)
{
	return (xQueueSendToBack(handle, (void *) &itemToQueue, ticksToWait) == pdTRUE) ? true : false;
}

/**
 * @brief		Sends item to queue back and waits the default ticks
 *
 * @param		itemToQueue		Item to push to back
 * @return		True if it was successful, false otherwise
 *
 * @details		Sends an item to the back of the queue and waits the default
 * 				amount of time. The default value is `0`, so it waits the
 * 				minimum of time.
 * @see			https://www.freertos.org/xQueueSendToBack.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- 21.04.2023	NZ	Mod: That the overloaded function calls
 * 									it's "default" method
 ****************************************************************************/
template <typename T>
inline bool Queue<T>::sendToBack(const T itemToQueue)
{
	return Queue<T>::sendToBack(itemToQueue, defaultMinTicksToWait);
}

/**
 * @brief		Sends item to queue back from an ISR
 *
 * @param		itemToQueue		Item to push to back
 * @return		True if it was successful, false otherwise
 *
 * @details		Sends an item to the back of the queue from an interrupt
 * 				service routine, it has no delay because it doesn't block.
 * @warning		The `pxHigherPriorityTaskWoken` is an optional parameter
 * 				and is constant set to `NULL`.
 * @see			https://www.freertos.org/xQueueSendToBackFromISR.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline bool Queue<T>::sendToBackFromISR(const T itemToQueue)
{
	return (xQueueSendToBackFromISR(handle, (void *) &itemToQueue, NULL) == pdTRUE) ? true : false;
}

/**
 * @brief		Sends item to queue front and waits the given ticks
 *
 * @param		itemToQueue		Item to push to front
 * @param		ticksToWait		Ticks to wait to complete
 * @return		True if it was successful, false otherwise
 *
 * @details		Sends an item to the front of the queue and waits the given
 * 				amount of time. For the maximum of wait time use the macro
 * 				`portMAX_DELAY` or the function `sendToBack(const void *itemToQueue)`.
 * @see			https://www.freertos.org/xQueueSendToFront.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline bool Queue<T>::sendToFront(const T itemToQueue, TickType_t ticksToWait)
{
	return (xQueueSendToBack(handle, (void *) &itemToQueue, ticksToWait) == pdTRUE) ? true : false;
}

/**
 * @brief		Sends item to queue front and waits the default ticks
 *
 * @param		itemToQueue		Item to push to front
 * @return		True if it was successful, false otherwise
 *
 * @details		Sends an item to the front of the queue and waits the default
 * 				amount of time. The default value is `0`, so it waits the
 * 				minimum of time.
 * @see			https://www.freertos.org/xQueueSendToFront.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- 21.04.2023	NZ	Mod: That the overloaded function calls
 * 									it's "default" method
 ****************************************************************************/
template <typename T>
inline bool Queue<T>::sendToFront(const T itemToQueue)
{
	return Queue<T>::sendToFront(itemToQueue, defaultMinTicksToWait);
}

/**
 * @brief		Sends item to queue front from an ISR
 *
 * @param		itemToQueue		Item to push to front
 * @return		True if it was successful, false otherwise
 *
 * @details		Sends an item to the front of the queue from an interrupt
 * 				service routine, it has no delay because it doesn't block.
 * @warning		The `pxHigherPriorityTaskWoken` is an optional parameter
 * 				and is constant set to `NULL`.
 * @see			https://www.freertos.org/xQueueSendToFrontFromISR.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline bool Queue<T>::sendToFrontFromISR(const T itemToQueue)
{
	return (xQueueSendToBackFromISR(handle, (void *) &itemToQueue, NULL) == pdTRUE) ? true : false;
}

/**
 * @brief		Receives items to a defined buffer and waits the given ticks
 *
 * @param		ticksToWait		Ticks to wait to complete
 * @return		The start of the message as `void` pointer
 *
 * @details		Receives an item form the queue, waits the given amount of
 * 				time and blocks the task. It's create a local buffer and
 * 				returns the item of the queue.
 * @see			https://www.freertos.org/a00118.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline T Queue<T>::receive(TickType_t ticksToWait)
{
	T buffer; // Create local buffer
	BaseType_t ret; // Temporary return value

	ret = xQueueReceive(handle, &buffer, ticksToWait);

	assert(ret == pdTRUE);

	return buffer;
}

/**
 * @brief		Receives items to a defined buffer and waits the default ticks
 *
 * @param		ticksToWait		Ticks to wait to complete
 * @return		The start of the message as `void` pointer
 *
 * @details		Receives an item form the queue, waits the default amount of
 * 				time. The default value is `portMAX_DELAY`, so it waits the
 * 				maximum of time. It's create a local buffer and returns the
 * 				item of the queue.
 * @see			https://www.freertos.org/a00118.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- 21.04.2023	NZ	Mod: That the overloaded function calls
 * 									it's "default" method
 ****************************************************************************/
template <typename T>
inline T Queue<T>::receive(void)
{
	return Queue<T>::receive(defaultMaxTicksToWait);;
}

/**
 * @brief		Receives items to a defined buffer from an ISR
 *
 * @param		buffer			Buffer to store the message in
 * @param		ticksToWait		Ticks to wait to complete
 * @return		The start of the message as `void` pointer
 *
 * @details		Receives an item form the queue, from an interrupt service
 * 				routine, it has no delay because it doesn't block. It's create
 * 				a local buffer and returns the item of the queue.
 * @see			https://www.freertos.org/a00120.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline T Queue<T>::receiveFromISR(void)
{
	T buffer; // Create local buffer

	xQueueReceiveFromISR(handle, &buffer, NULL);

	return buffer;
}

/**
 * @brief		Return the number of messages stored in a queue
 *
 * @param		void
 * @return		Number of messages stored in a queue
 *
 * @details		Return the number of messages stored in a queue like the
 * 				FreeRTOS API function.
 * @see			https://www.freertos.org/a00018.html#ucQueueMessagesWaiting
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline UBaseType_t Queue<T>::messagesWaiting(void)
{
	return uxQueueMessagesWaiting(handle);
}

/**
 * @brief		Return the number of messages stored in a queue from ISR
 *
 * @param		void
 * @return		Number of messages stored in a queue
 *
 * @details		Return the number of messages stored in a queue, requested from
 * 				an interrup service routine like the FreeRTOS API function.
 * @see			https://www.freertos.org/a00018.html#ucQueueMessagesWaitingFromISR
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline UBaseType_t Queue<T>::messagesWaitingFromISR(void)
{
	return uxQueueMessagesWaitingFromISR(handle);
}

/**
 * @brief		Return the number of free spaces in a queue
 *
 * @param		void
 * @return		Number of free spaces in a queue
 *
 * @details		Return the number of messages stored in a queue like the
 * 				FreeRTOS API function.
 * @see			https://www.freertos.org/a00018.html#uxQueueSpacesAvailable
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline UBaseType_t Queue<T>::spacesAvailable(void)
{
	return uxQueueSpacesAvailable(handle);
}

/**
 * @brief		Used to set the default max ticks
 *
 * @param		newTicksToWait	New value for the `defaultMaxTicksToWait`
 * @return		void
 *
 * @details		Sets the new default ticks to the given value. This
 * 				`defaultMaxTicksToWait` is used when no value for the
 * 				parameter `ticksToWait` is given. The value is initialized
 * 				to `portMAX_DELAY`.
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- 17.04.2023	NZ	Mod: Method to setDefaultMaxTicksToWait()
 ****************************************************************************/
template <typename T>
inline void Queue<T>::setDefaultMaxTicksToWait(TickType_t newTicksToWait)
{
	defaultMaxTicksToWait = newTicksToWait;
}

/**
 * @brief		Used to set the default min ticks
 *
 * @param		newTicksToWait	New value for the `defaultMinTicksToWait`
 * @return		void
 *
 * @details		Sets the new default ticks to the given value. This
 * 				`defaultMinTicksToWait` is used when no value for the
 * 				parameter `ticksToWait` is given. The value is initialized
 * 				to `0`.
 *
 * @author		N. Zoller (NZ)
 * @date		17.04.2023	NZ	Created
 ****************************************************************************/
template <typename T>
inline void Queue<T>::setDefaultMinTicksToWait(TickType_t newTicksToWait)
{
	defaultMinTicksToWait = newTicksToWait;
}

/****************************************************************************/
/* End Header : Queue Class													*/
/****************************************************************************/
#endif /* QUEUE_HPP_ */
