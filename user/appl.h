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
#include "thread/entrant.h"
#include "thread/scheduler.h"

class Application : public Entrant
 
{

public:
	Application (const Application &copy) = delete; // prevent copying
	Application& operator=(const Application&) = delete; // prevent assignment
/* Add your code here */ 
	Application(void* tos) : Entrant(tos){}
	void action () override;
};

#endif
