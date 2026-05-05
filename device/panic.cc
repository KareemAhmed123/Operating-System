/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 P A N I C                                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Default interrupt handler.                                                */
/*****************************************************************************/
/* Add your code here */ 
/* Add your code here */ 
#include "device/panic.h"

Panic panic;

void Panic::trigger(){
	extern CGA_Stream kout;
	extern CPU cpu;

    kout << "PANIC: Unhandled interrupt!" << endl;
	cpu.halt();
}
