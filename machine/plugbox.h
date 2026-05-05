/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              P L U G B O X                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Abstracts an interrupt vector table. Allows to configure handler routines */
/* for every hardware and software interrupt and every CPU exception.        */
/*****************************************************************************/

#ifndef __Plugbox_include__
#define __Plugbox_include__

#include "guard/gate.h"
#include "device/panic.h"

class Plugbox {
public:
	Plugbox(const Plugbox &copy) = delete; // prevent copying
	Plugbox& operator=(const Plugbox&) = delete; // prevent assignment
	
	/* Add your code here */ 
	
	enum {
        timer = 32,
        keyboard = 33,
        size = 64
    };
    
	Plugbox();
	
	void assign ( unsigned int slot, Gate& gate);
	
	Gate& report (unsigned int slot);


private:
/* Add your code here */
	Gate* gates[size]; 
	
};

#endif
