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

/* Add your code here */ 
#include "machine/plugbox.h"

extern Panic panic;

//Initalises every Gate as Panic
Plugbox::Plugbox(){
	for (int i = 0; i < size; i++){
		gates[i] = &panic;
	}
}

//assigns a Gate to a Slot
void Plugbox::assign(unsigned int slot, Gate& gate){
	gates[slot] = &gate;
}

//retrives a Gate for a given Slot
Gate& Plugbox::report(unsigned int slot){
	return *gates[slot];
}
