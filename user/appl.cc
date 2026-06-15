/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Test application for demonstrating synchronization.                        */
/*****************************************************************************/

/* INCLUDES */
#include "user/appl.h"
#include "device/cgastr.h"
#include "user/loop.h"


/*
 * Global output stream.
 *
 * kout is the shared screen output object.
 *
 * Both the normal application and keyboard epilogues can use kout.
 * That is why output operations must be protected in the fixed mode.
 */
extern CGA_Stream kout;
extern Guard guard;
extern Scheduler scheduler;


void Application::action()
{
/* Add your code here */ 

	static char stack_1[4096];
	static char stack_2[4096];
	static char stack_3[4096];
	static char stack_4[4096];
	static char stack_5[4096];
	
    Loop l_1(stack_1 + sizeof(stack_1));
    Loop l_2(stack_2 + sizeof(stack_2));
    Loop l_3(stack_3 + sizeof(stack_3));
    Loop l_4(stack_4 + sizeof(stack_4));
    Loop l_5(stack_5 + sizeof(stack_5));
	scheduler.ready(l_1);
	scheduler.ready(l_2);
	scheduler.ready(l_3);
	scheduler.ready(l_4);
	scheduler.ready(l_5);
	scheduler.resume();
	for(int r=0; r<10; r++){

		scheduler.resume();
	}
	scheduler.kill(l_3);
	while(1){scheduler.resume();}
    kout << endl << "Back in Application. Test finished." << endl;
}
