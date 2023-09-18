#ifndef TIMER_HPP_
#define TIMER_HPP_
/****************************************************************************/
/*  Header    : Timer Class													*/
/****************************************************************************/
/*                                                                          */
/*  @file     : Timer.hpp													*/
/*                                                                          */
/*  @brief	  : FreeRTOS-Timer Wrapper class								*/
/*                                                                          */
/*  @author   : N. Zoller (NZ)                                              */
/*                                                                          */
/*  @date	  : 08.04.2023  NZ	Created                              		*/
/*                                                                          */
/*	@remark	  : Last Modifications:											*/
/* 				- 10.04.2023	NZ	Mod: Header, add doxygen commands		*/
/*				- 21.04.2023	NZ	Mod: Combined .cpp and .hpp and made the*/
/*									methods inline							*/
/*																			*/
/*  @todo	  :	- Add this functions from API								*/
/*					- xTimerCreateStatic() 									*/
/*					- xTimerStartFromISR() 									*/
/*					- xTimerStopFromISR() 									*/
/*					- xTimerChangePeriodFromISR()							*/
/*					- xTimerResetFromISR() 									*/
/*					- vTimerSetReloadMode()									*/
/*					- pcTimerGetName()	 									*/
/*					- xTimerGetExpiryTime()									*/
/*					- xTimerGetReloadMode()									*/
/*				- Test the whole class extensively							*/
/*																			*/
/****************************************************************************/
/*																			*/
/****************************************************************************/

/* imports */
#include <string>

#include <timers.h>

/* Class constant declaration  */

/* Class Type declaration      */

/* Class data declaration      */

/* Class definition            */
class Timer : public FreeRTOS
{
protected:
	TimerHandle_t					handle;
	const std::string				name;
	const TimerCallbackFunction_t	callbackFunc;

	TickType_t						defaultBlockTime = 0;

public:
	Timer(std::string timerName, TickType_t timerPeriod, bool timerAutoReload, int timerID, TimerCallbackFunction_t timerCallbackFunc);
	Timer(std::string timerName, TickType_t timerPeriod, bool timerAutoReload, TimerCallbackFunction_t timerCallbackFunc);

	~Timer(void);

	bool isActive(void);

	bool start(TickType_t blockTime);
	bool start(void);

	bool stop(TickType_t blockTime);
	bool stop(void);

	bool reset(TickType_t blockTime);
	bool reset(void);

	bool setPeriod(TickType_t newPeriod, TickType_t blockTime);
	bool setPeriod(TickType_t newPeriod);

	TickType_t getPeriod(void);

	void setID(int newID);
	int getID(void);

	std::string getName(void);

	void setDefaultBlockTime(TickType_t newBlockTime);
	TickType_t getDefaultBlockTime(void);
};

/**
 * @brief		Constructor
 *
 * @param		timerName			A human readable text name for the timer
 * @param		timerPeriod			Period of the timer in ticks
 * @param		timerAutoReload		Set to true the timer will auto reload it self
 * @param		timerID				An identifier that is assigned to the timer
 * @param		timerCallbackFunc	The function to call when the timer expires
 *
 * @details		Constructs a new timer object with the FreeRTOS API function
 * 				xTimerCreate().
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerCreate.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline Timer::Timer(std::string timerName, TickType_t timerPeriod, bool timerAutoReload, int timerID, TimerCallbackFunction_t timerCallbackFunc)
			:	name(timerName),
				callbackFunc(timerCallbackFunc)
{
	handle = xTimerCreate(name.c_str(), timerPeriod, (int) timerAutoReload, (void *) timerID, callbackFunc);

	assert(handle != NULL);
};

/**
 * @brief		Constructor (default ID = 0)
 *
 * @param		timerName			A human readable text name for the timer
 * @param		timerPeriod			Period of the timer in ticks
 * @param		timerAutoReload		Set to true the timer will auto reload it self
 * @param		timerCallbackFunc	The function to call when the timer expires
 *
 * @details		Constructs a new timer object with the FreeRTOS API function
 * 				xTimerCreate() with an default timerID of 0.
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerCreate.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline Timer::Timer(std::string timerName, TickType_t timerPeriod, bool timerAutoReload, TimerCallbackFunction_t timerCallbackFunc)
			:	name(timerName),
				callbackFunc(timerCallbackFunc)
{
	handle = xTimerCreate(name.c_str(), timerPeriod, (int) timerAutoReload, (void *) 0, callbackFunc);

	assert(handle != NULL);
};

/**
 * @brief		Destructor
 *
 * @param		void
 *
 * @details		Deletes a timer object with the FreeRTOS API function
 * 				xTimerDelete(), waits the default block time.
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerDelete.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline Timer::~Timer(void)
{
	BaseType_t ret; // Temporary return value

	ret = xTimerDelete(handle, defaultBlockTime);

	assert(ret == pdPASS);
};

/**
 * @brief		Checks if timer is dormant
 *
 * @param		void
 * return		False if it is dormant, true otherwise
 *
 * @details		This function checks if the timer is active or dormant.
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerIsTimerActive.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline bool Timer::isActive(void)
{
	return (xTimerIsTimerActive(handle) == pdFALSE) ? false : true;
}

/**
 * @brief		Starts the timer, with the given block time
 *
 * @param		blockTime		Number of ticks to held the task in block state
 * return		True if it was successful, false otherwise
 *
 * @details		This function starts the timer and waits the given block time
 * 				for the start command to be successfully sent to the timer.
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerStart.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline bool Timer::start(TickType_t blockTime)
{
	return (xTimerStart(handle, blockTime) == pdTRUE) ? true : false;
}

/**
 * @brief		Starts the timer, with the default block time
 *
 * @param		void
 * return		True if it was successful, false otherwise
 *
 * @details		This function starts the timer and waits the default block time
 * 				for the start command to be successfully sent to the timer.
 * 				The default block time is initialized with 0 and could changed
 * 				afterwards.
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerStart.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- 21.04.2023	NZ	Mod: That the overloaded function calls
 * 									it's "default" method
 ****************************************************************************/
inline bool Timer::start(void)
{
	return Timer::start(defaultBlockTime);
}

/**
 * @brief		Stops the timer, with the given block time
 *
 * @param		blockTime		Number of ticks to held the task in block state
 * return		True if it was successful, false otherwise
 *
 * @details		This function stops the timer and waits the given block time
 * 				for the stop command to be successfully sent to the timer.
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerStop.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline bool Timer::stop(TickType_t blockTime)
{
	return (xTimerStop(handle, blockTime) == pdTRUE) ? true : false;
}

/**
 * @brief		Stops the timer, with the default block time
 *
 * @param		void
 * return		True if it was successful, false otherwise
 *
 * @details		This function stops the timer and waits the default block time
 * 				for the stop command to be successfully sent to the timer.
 * 				The default block time is initialized with 0 and could changed
 * 				afterwards.
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerStop.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- 21.04.2023	NZ	Mod: That the overloaded function calls
 * 									it's "default" method
 ****************************************************************************/
inline bool Timer::stop(void)
{
	return Timer::stop(defaultBlockTime);
}

/**
 * @brief		Resets the timer, with the given block time
 *
 * @param		blockTime		Number of ticks to held the task in block state
 * return		True if it was successful, false otherwise
 *
 * @details		This function resets the timer and waits the given block time
 * 				for the reset command to be successfully sent to the timer.
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerReset.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline bool Timer::reset(TickType_t blockTime)
{
	return (xTimerReset(handle, blockTime) == pdTRUE) ? true : false;
}

/**
 * @brief		Resets the timer, with the default block time
 *
 * @param		void
 * return		True if it was successful, false otherwise
 *
 * @details		This function resets the timer and waits the default block time
 * 				for the reset command to be successfully sent to the timer.
 * 				The default block time is initialized with 0 and could changed
 * 				afterwards.
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerReset.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- 21.04.2023	NZ	Mod: That the overloaded function calls
 * 									it's "default" method
 ****************************************************************************/
inline bool Timer::reset(void)
{
	return Timer::reset(defaultBlockTime);
}

/**
 * @brief		Sets the period of the timer, with the given block time
 *
 * @param		newPeriod		New period in ticks
 * @param		blockTime		Number of ticks to held the task in block state
 * return		True if it was successful, false otherwise
 *
 * @details		This function sets the period of the timer and waits the given
 * 				block time for the reset command to be successfully sent to
 * 				the timer.
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerChangePeriod.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline bool Timer::setPeriod(TickType_t newPeriod, TickType_t blockTime)
{
	return (xTimerChangePeriod(handle, newPeriod, blockTime) == pdTRUE) ? true : false;
}

/**
 * @brief		Sets the period of the timer, with the default block time
 *
 * @param		newPeriod		New period in ticks
 * return		True if it was successful, false otherwise
 *
 * @details		This function sets the period of the timer and waits the given
 * 				block time for the reset command to be successfully sent to
 * 				the timer. The default block time is initialized with 0 and
 * 				could changed afterwards.
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerChangePeriod.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- 21.04.2023	NZ	Mod: That the overloaded function calls
 * 									it's "default" method
 ****************************************************************************/
inline bool Timer::setPeriod(TickType_t newPeriod)
{
	return Timer::setPeriod(newPeriod, defaultBlockTime);
}

/**
 * @brief		Gets the period of the timer
 *
 * @param		void
 * return		The period of the timer, in ticks
 *
 * @details		This function gets the period of the timer set initially or
 * 				changed with the method `setPeriod()`.
 * @see			https://www.freertos.org/FreeRTOS-timers-xTimerGetPeriod.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline TickType_t Timer::getPeriod(void)
{
	return xTimerGetPeriod(handle);
}

/**
 * @brief		Sets the id of the timer
 *
 * @param		newID		New id for the timer
 * return		void
 *
 * @details		This function sets the id of the timer
 * @see			https://www.freertos.org/FreeRTOS-timers-vTimerSetTimerID.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline void Timer::setID(int newID)
{
	vTimerSetTimerID(handle, (void *) newID);
}

/**
 * @brief		Gets the id of the timer
 *
 * @param		void
 * return		Returns the current ID of the timer
 *
 * @details		This function gets the id of the timer
 * @see			https://www.freertos.org/FreeRTOS-timers-pvTimerGetTimerID.html
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline int Timer::getID(void)
{
	return (int) pvTimerGetTimerID(handle);
}

/**
 * @brief		Gets the name of the timer
 *
 * @param		void
 * return		Returns the name of the timer
 *
 * @details		This function gets the name of the timer from the class,
 * 				not the timer struc of FreeRTOS.
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline std::string Timer::getName(void)
{
	return pcTimerGetName(handle);
}

/**
 * @brief		Sets the default block time of the timer
 *
 * @param		newBlockTime	New default block time for this timer
 * return		void
 *
 * @details		This function sets the default block time of the timer
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline void Timer::setDefaultBlockTime(TickType_t newBlockTime)
{
	defaultBlockTime = newBlockTime;
}

/**
 * @brief		Gets the default block time of the timer
 *
 * @param		void
 * return		Returns the current default block time of the timer, in ticks
 *
 * @details		This function gets the default block time of the timer
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline TickType_t Timer::getDefaultBlockTime(void)
{
	return defaultBlockTime;
}

/****************************************************************************/
/* End Header : Timer Class													*/
/****************************************************************************/
#endif /* TIMER_HPP_ */
