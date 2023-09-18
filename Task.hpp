#ifndef TASK_HPP_
#define TASK_HPP_
/****************************************************************************/
/*  Header    : Task Class													*/
/****************************************************************************/
/*                                                                          */
/*  @file     : Task.hpp													*/
/*                                                                          */
/*  @brief	  : FreeRTOS-Task Wrapper class									*/
/*                                                                          */
/*  @author   : N. Zoller (NZ)                                              */
/*                                                                          */
/*  @date	  : 08.04.2023  NZ	Created                              		*/
/*                                                                          */
/*	@remark	  : Last Modifications:											*/
/* 				- 10.04.2023	NZ	Mod: Header, add doxygen commands		*/
/*				- 17.40.2023	NZ	Mod: Moved the methods in the .cpp file	*/
/*									and add method headers					*/
/*				- 21.04.2023	NZ	Mod: Combined .cpp and .hpp and made the*/
/*									methods inline							*/
/*                                                                          */
/*  @todo	  :	- Add this functions from API								*/
/*					- xTaskCreateStatic()									*/
/*					- xTaskCreateRestrictedStatic()							*/
/*					- xTaskDelayUntil()										*/
/*					- xTaskAbortDelay()										*/
/*					- uxTaskGetSystemState()								*/
/*					- xTaskGetApplicationTaskTag()							*/
/*					- xTaskGetCurrentTaskHandle()							*/
/*					- uxTaskGetStackHighWaterMark()							*/
/*					- pcTaskGetName()										*/
/*				- Test the whole class extensively							*/
/*																			*/
/****************************************************************************/
/*																			*/
/****************************************************************************/

/* imports */
#include <string>

#include <FreeRTOS.h>
#include <task.h>

/* Class constant declaration  */

/* Class Type declaration      */

/* Class data declaration      */

/* Class definition            */
class Task : public FreeRTOS
{
protected:
	const TaskFunction_t			functionPointer;
	const std::string				name;
	const configSTACK_DEPTH_TYPE	stackSize;
	const void						*parameters;
	TaskHandle_t					handle = NULL;

	TickType_t						currentTickCount;

	TaskStatus_t					status;
	BaseType_t						freeStackSpace;

public:
	Task(	TaskFunction_t taskFunction,
			std::string taskName,
			configSTACK_DEPTH_TYPE taskStackSize,
			void *taskParameters,
            UBaseType_t taskPriority);

	Task(	TaskFunction_t taskFunction,
			const std::string taskName,
			configSTACK_DEPTH_TYPE taskStackSize,
			UBaseType_t taskPriority);

	~Task();

	void delay(const TickType_t ticksToDelay);

	void delayUntil(TickType_t *previousWakeTime, const TickType_t ticksToIncrement);
	void delayUntil(const TickType_t ticksToIncrement);

	UBaseType_t getPriority(void);

	void setPriority(UBaseType_t newPriority);

	void suspend(void);

	void resume(void);

	bool resumeFromISR(void);

	void yield(void);

	TaskStatus_t getInfo(eTaskState eState);
	TaskStatus_t getInfo(void);

	BaseType_t getFreeStackSpace(void);

	eTaskState getState(void);

	TickType_t getTickCount(void);

	TickType_t updateTickCount(void);

	TickType_t updateTickCountFromISR(void);

	std::string getName(void);
};

/**
 * @brief		Constructor
 *
 * @param		taskFunction	Pointer to the task entry function
 * @param		taskName		A descriptive name for the task
 * @param		taskStackSize	The number of words (not bytes!) to allocate for use as the task's stack
 * @param		taskParameters	A value that is passed as the paramater to the created task
 * @param		taskPriority	The priority at which the created task will execute
 * @param		taskHandle		Used to pass a handle to the created task out of the xTaskCreate() function
 *
 * @details		Constructs a new queue object with the FreeRTOS API function
 * 				xTaskCreate().
 * @see			https://www.freertos.org/a00125.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline Task::Task(	TaskFunction_t taskFunction,
					std::string taskName,
					configSTACK_DEPTH_TYPE taskStackSize,
					void *taskParameters,
					UBaseType_t taskPriority)
					:	functionPointer(taskFunction),
						name(taskName),
						stackSize(taskStackSize),
						parameters(taskParameters)
{
	BaseType_t ret; // Temporary return value

	ret = xTaskCreate(functionPointer, (const char * const) name.c_str(),  stackSize, (void *) parameters, taskPriority, &handle);

	assert(ret == pdPASS);
	assert(handle != NULL);
}

/**
 * @brief		Constructor, with zero as parameters
 *
 * @param		taskFunction	Pointer to the task entry function
 * @param		taskName		A descriptive name for the task
 * @param		taskStackSize	The number of words (not bytes!) to allocate for use as the task's stack
 * @param		taskPriority	The priority at which the created task will execute
 * @param		taskHandle		Used to pass a handle to the created task out of the xTaskCreate() function
 *
 * @details		Constructs a new queue object with the FreeRTOS API function
 * 				xTaskCreate(). And sets the parameters to zero.
 * @see			https://www.freertos.org/a00125.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline Task::Task(	TaskFunction_t taskFunction,
					std::string taskName,
					configSTACK_DEPTH_TYPE taskStackSize,
					UBaseType_t taskPriority)
					:	functionPointer(taskFunction),
						name(taskName),
						stackSize(taskStackSize)
{
	BaseType_t ret; // Temporary return value

	ret = xTaskCreate(functionPointer, (const char * const) name.c_str(),  stackSize, (void *) 0, taskPriority, &handle);

	assert(ret == pdPASS);
	assert(handle != NULL);
}

/**
 * @brief		Destructor
 *
 * @param		void
 *
 * @details		Deletes a queue object with the FreeRTOS API function
 * 				vTaskDelete().
 * @see			https://www.freertos.org/a00126.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline Task::~Task(void)
{
	vTaskDelete(handle);;
};

/**
 * @brief		Delay a task
 *
 * @param		ticksToDelay	The amount of time, in tick periods, that the calling task should block
 * @return		void
 *
 * @details		Delay a task for a given number of ticks.The actual time that
 * 				the task remains blocked depends on the tick rate.
 * @see			https://www.freertos.org/a00127.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline void Task::delay(const TickType_t ticksToDelay)
{
	vTaskDelay(ticksToDelay);
}

/**
 * @brief		Delay a task until a specified time
 *
 * @param		previousWakeTime	Pointer to a variable that holds the time at which the task was last unblocked
 * @param		ticksToIncrement	The cycle time period.
 * @return		void
 *
 * @details		Delay a task until a specified time. This function can be
 * 				used by periodic tasks to ensure a constant execution frequency.
 * @see			https://www.freertos.org/vtaskdelayuntil.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline void Task::delayUntil(TickType_t *previousWakeTime, const TickType_t ticksToIncrement)
{
	vTaskDelayUntil(previousWakeTime, ticksToIncrement);
}

/**
 * @brief		Delay a task until a specified time, without previousWakeTime
 *
 * @param		ticksToIncrement	The cycle time period.
 * @return		void
 *
 * @details		Delay a task until a specified time. This function can be
 * 				used by periodic tasks to ensure a constant execution frequency.
 * @see			https://www.freertos.org/vtaskdelayuntil.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 * @remark		Last Modifications:
 * 				- 21.04.2023	NZ	Mod: That the overloaded function calls
 * 									it's "default" method
 ****************************************************************************/
inline void Task::delayUntil(const TickType_t ticksToIncrement)
{
	Task::delayUntil(&currentTickCount, ticksToIncrement);
}

/**
 * @brief		Obtain the priority
 *
 * @param		void
 * @return		The priority of the task.
 *
 * @details		Gets the priority of the current task.
 * @see			https://www.freertos.org/a00128.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline UBaseType_t Task::getPriority(void)
{
	return uxTaskPriorityGet(NULL);
}

/**
 * @brief		Set the priority
 *
 * @param		newPriority			The priority to which the task will be set
 * @return		void
 *
 * @details		Sets the priority of the current task.
 * @see			https://www.freertos.org/a00129.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline void Task::setPriority(UBaseType_t newPriority)
{
	vTaskPrioritySet(handle, newPriority);
}

/**
 * @brief		Suspend task
 *
 * @param		void
 * @return		void
 *
 * @details		Suspend current task. When suspended a task will never get
 * 				any microcontroller processing time, no matter what its priority.
 * @see			https://www.freertos.org/a00130.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline void Task::suspend(void)
{
	vTaskSuspend(handle);
}

/**
 * @brief		Resume task
 *
 * @param		void
 * @return		void
 *
 * @details		A task that has been suspended by one or more calls to
 * 				vTaskSuspend() will be made available for running again by a
 * 				single call to vTaskResume().
 * @see			https://www.freertos.org/a00131.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline void Task::resume(void)
{
	vTaskResume(handle);
}

/**
 * @brief		Resume task from ISR
 *
 * @param		void
 * @return		True if it was successful, false otherwise
 *
 * @details		A task that has been suspended by one of more calls to
 * 				vTaskSuspend() will be made available for running again by a
 * 				single call to xTaskResumeFromISR().
 * @see			https://www.freertos.org/taskresumefromisr.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline bool Task::resumeFromISR(void)
{
	return (xTaskResumeFromISR(handle) == pdTRUE) ? true : false;
}

/**
 * @brief		Request a context switch
 *
 * @param		void
 * @return		True if it was successful, false otherwise
 *
 * @details		taskYIELD() is used to request a context switch to another task.
 * @see			https://www.freertos.org/a00020.html#taskYIELD
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline void Task::yield(void)
{
	taskYIELD();
}

/**
 * @brief		Gets the task infos
 *
 * @param		eState				Manually sets the eState option
 * @return		Returns the status from the vTaskGetInfo()
 *
 * @details		Gets the TaskStatus_t structure for the current task. The
 * 				TaskStatus_t structure contains, among other things, members
 * 				for the task handle, task name, task priority, task state,
 * 				and total amount of run time consumed by the task.
 * @see			https://www.freertos.org/vTaskGetInfo.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline TaskStatus_t Task::getInfo(eTaskState eState)
{
	vTaskGetInfo(handle, &status, freeStackSpace, eState);

	return status;
}

/**
 * @brief		Gets the task infos, with eState included
 *
 * @param		void
 * @return		Returns the status from the vTaskGetInfo()
 *
 * @details		Gets the TaskStatus_t structure for the current task. The
 * 				TaskStatus_t structure contains, among other things, members
 * 				for the task handle, task name, task priority, task state,
 * 				and total amount of run time consumed by the task. With the
 * 				eState included as default.
 * @warning		eState used, obtaining the task state is not as fast as a
 * 				simple assignment, so it's uses more time.
 * @see			https://www.freertos.org/vTaskGetInfo.html
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline TaskStatus_t Task::getInfo(void)
{
	vTaskGetInfo(handle, &status, freeStackSpace, eInvalid);

	return status;
}

/**
 * @brief		Gets the free stack space
 *
 * @param		void
 * @return		Returns the free stack space from the vTaskGetInfo()
 *
 * @details		Gets the free stack space from the vTaskGetInfo().
 * @warning		This method also updates the status and the freeStackSpace,
 * 				because it calls the function getInfo() before return the
 * 				freeStackSpace.
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline BaseType_t Task::getFreeStackSpace(void)
{
	getInfo();
	return freeStackSpace;
}

/**
 * @brief		Gets the task state
 *
 * @param		void
 * @return		Returns the state of the task
 *
 * @details		Returns as an enumerated type the state in which a task
 * 				existed at the time getState() was executed.
 * @see			https://www.freertos.org/a00021.html#eTaskGetState
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline eTaskState  Task::getState(void)
{
	return eTaskGetState(handle);
}

/**
 * @brief		Get tick count
 *
 * @param		void
 * @return		Returns the tick count
 *
 * @details		The count of ticks since vTaskStartScheduler was called,
 * 				last updated when updateTickCount() was called.
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline TickType_t  Task::getTickCount(void)
{
	return currentTickCount;
}

/**
 * @brief		Updates and returns the current tick count
 *
 * @param		void
 * @return		Returns the current tick count
 *
 * @details		The current count of ticks since vTaskStartScheduler was
 * 				called.
 * @warning		This function cannot be called from an ISR.
 * @see			https://www.freertos.org/a00021.html#xTaskGetTickCount
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline TickType_t  Task::updateTickCount(void)
{
	currentTickCount = xTaskGetTickCount();
	return currentTickCount;
}

/**
 * @brief		Updates and returns the current tick count, from ISR
 *
 * @param		void
 * @return		Returns the current tick count
 *
 * @details		The current count of ticks since vTaskStartScheduler was
 * 				called, from an interrupt service routine.
 * @see			https://www.freertos.org/a00021.html#xTaskGetTickCount
 *
 * @author		N. Zoller (NZ)
 * @date		14.04.2023	NZ	Created
 ****************************************************************************/
inline TickType_t  Task::updateTickCountFromISR(void)
{
	currentTickCount = xTaskGetTickCountFromISR();
	return currentTickCount;
}

/**
 * @brief		Gets the name of the task
 *
 * @param		void
 * return		Returns the name of the task
 *
 * @details		This function gets the name of the task from the class,
 * 				not from the task of FreeRTOS.
 *
 * @author		N. Zoller (NZ)
 * @date		10.04.2023	NZ	Created
 ****************************************************************************/
inline std::string Task::getName(void)
{
	return pcTaskGetName(handle);
}

/****************************************************************************/
/* End Header : Task Class													*/
/****************************************************************************/
#endif /* TASK_HPP_ */
