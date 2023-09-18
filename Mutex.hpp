#ifndef MUTEX_HPP_
#define MUTEX_HPP_
/****************************************************************************/
/*  Header    : Mutex Class													*/
/****************************************************************************/
/*                                                                          */
/*  @file     : Mutex.hpp													*/
/*                                                                          */
/*  @brief	  : FreeRTOS-Mutex Wrapper class								*/
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
class Mutex : public Semaphore
{
public:
	Mutex(void): Semaphore() {};

	~Mutex(void);
};
#endif /* MUTEX_HPP_ */
