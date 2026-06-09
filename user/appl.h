/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Application class defines the (only) application for OOStuBS.         */
/*****************************************************************************/

#ifndef __application_include__
#define __application_include__

#include "guard/secure.h"
#include "thread/coroutine.h"

class Application : public Coroutine
 
{

public:
	Application (const Application &copy) = delete; // prevent copying
	Application& operator=(const Application&) = delete; // prevent assignment
/* Add your code here */ 
	Application(void* tos) : Coroutine(tos){}
	void action () override;
};

#endif
