#ifndef FREERTOS_HPP_
#define FREERTOS_HPP_
/****************************************************************************/
/*  Header    : FreeRTOS Class												*/
/****************************************************************************/
/*                                                                          */
/*  @file     : FreeRTOS.hpp                                                */
/*                                                                          */
/*  @brief	  : FreeRTOS Wrapper class										*/
/*                                                                          */
/*  @author   : N. Zoller (NZ)                                              */
/*                                                                          */
/*  @date	  : 08.04.2023  NZ	Created                              		*/
/*                                                                          */
/*	@remark	  : Last Modifications:											*/
/* 				- 10.04.2023	NZ	Mod: Header, add doxygen commands		*/
/* 				- 24.04.2023	NZ	Mod: Moved some methods from declaration*/
/*									down to the definition section			*/
/*																			*/
/*  @todo	  :	- Add this functions from API								*/
/*					- uxTaskPriorityGet()									*/
/*					- xTaskGetHandle()										*/
/*					- xTaskGetIdleTaskHandle()								*/
/*					- uxTaskGetStackHighWaterMark()							*/
/*					- eTaskGetState()										*/
/*					- xTaskGetSchedulerState()								*/
/*					- uxTaskGetNumberOfTasks()								*/
/*					- vTaskList()											*/
/*					- vTaskGetRunTimeStats()								*/
/*					- taskENTER_CRITICAL()									*/
/*					- taskEXIT_CRITICAL()									*/
/*					- taskENTER_CRITICAL_FROM_ISR()							*/
/*					- taskEXIT_CRITICAL_FROM_ISR()							*/
/*					- taskDISABLE_INTERRUPTS()								*/
/*					- taskENABLE_INTERRUPTS()								*/
/*					- vTaskStepTick()										*/
/*					- xTaskCatchUpTicks()									*/
/*																			*/
/****************************************************************************/
/*																			*/
/****************************************************************************/

/* imports */
#include <chrono>

#include <FreeRTOS.h>
#include <task.h>

/* Class constant declaration  */

/* Class Type declaration      */

/* Class data declaration      */

/* Class definition            */
class FreeRTOS
{
public:
	FreeRTOS() {};
	~FreeRTOS() {};

	void static startScheduler(void)										{vTaskStartScheduler();};

	void static endScheduler(void)											{vTaskEndScheduler();};

	void static suspendAll(void)											{vTaskSuspendAll();};

	bool static resumeAll(void);

	TickType_t static convertToTicks(const std::chrono::milliseconds msec);
	TickType_t static convertToTicks(const std::chrono::seconds sec);
	TickType_t static convertToTicks(const std::chrono::minutes minutes);
	TickType_t static convertToTicks(const std::chrono::hours hours);

	std::chrono::milliseconds static convertToTime(const TickType_t ticks);
};

/**
 * @brief		Resumes the scheduler
 *
 * @param		void
 * @return		True if it was successful, false otherwise
 *
 * @details		Resumes the scheduler after it was suspended using a call to
 * 				suspendAll().
 * @warning		resumeAll() only resumes the scheduler. It does not unsuspend
 * 				tasks that were previously suspended by a call to suspend().
 * @see			https://www.freertos.org/a00135.html
 *
 * @author		N. Zoller (NZ)
 * @date		24.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- none
 ****************************************************************************/
inline bool FreeRTOS::resumeAll(void)
{
	return (xTaskResumeAll() == pdTRUE) ? true : false;
}

/**
 * @brief		Convention to ticks from ms
 *
 * @param		msec			Time in milliseconds
 * @return		Time in ticks
 *
 * @details		Converts milliseconds from the `chrono`-Library to ticks.
 *
 * @author		N. Zoller (NZ)
 * @date		24.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- none
 ****************************************************************************/
inline TickType_t FreeRTOS::convertToTicks(const std::chrono::milliseconds msec)
{
	return msec.count() / portTICK_PERIOD_MS;
}

/**
 * @brief		Convention to ticks from s
 *
 * @param		sec			Time in seconds
 * @return		Time in ticks
 *
 * @details		Converts seconds from the `chrono`-Library to ticks.
 *
 * @author		N. Zoller (NZ)
 * @date		24.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- none
 ****************************************************************************/
inline TickType_t FreeRTOS::convertToTicks(const std::chrono::seconds sec)
{
	return sec.count() * 1000 / portTICK_PERIOD_MS;
}

/**
 * @brief		Convention to ticks from min
 *
 * @param		min			Time in minutes
 * @return		Time in ticks
 *
 * @details		Converts minutes from the `chrono`-Library to ticks.
 *
 * @author		N. Zoller (NZ)
 * @date		24.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- none
 ****************************************************************************/
inline TickType_t FreeRTOS::convertToTicks(const std::chrono::minutes minutes)
{
	return minutes.count() * 60 * 1000 / portTICK_PERIOD_MS;
}

/**
 * @brief		Convention to ticks from h
 *
 * @param		h			Time in hours
 * @return		Time in ticks
 *
 * @details		Converts hours from the `chrono`-Library to ticks.
 *
 * @author		N. Zoller (NZ)
 * @date		24.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- none
 ****************************************************************************/
inline TickType_t FreeRTOS::convertToTicks(const std::chrono::hours hours)
{
	return hours.count() * 60 * 60 * 1000 / portTICK_PERIOD_MS;
}

/**
 * @brief		Convention to ms from ticks
 *
 * @param		ticks			Time in ticks
 * @return		Time in milliseconds
 *
 * @details		Converts ticks from the FreeRTOS-Library to milliseconds from
 * 				the `chrono`-Library. From there other conventions from the
 * 				`chrono`-Library could be used.
 *
 * @author		N. Zoller (NZ)
 * @date		24.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- none
 ****************************************************************************/
inline std::chrono::milliseconds static convertToTime(const TickType_t ticks)
{
	return std::chrono::milliseconds(ticks * portTICK_PERIOD_MS);
}
/****************************************************************************/
/* End Header : FreeRTOS Class												*/
/****************************************************************************/
#endif /* FREERTOS_HPP_ */
