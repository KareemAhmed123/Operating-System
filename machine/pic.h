/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                  P I C                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Programmable Interrupt Controller.                                        */
/* By using the PIC, hardware interrupts can be enabled or suppressed        */
/* individually. This way we can control whether interrupts of a specific    */
/* device are forwarded to the CPU at all. Even then, the interrupt handler  */
/* gets only activated if the CPU is ready to react to interrupts. This can  */
/* be controlled by using class CPU.                                         */
/*****************************************************************************/

#ifndef __pic_include__
#define __pic_include__
#include "machine/io_port.h"
class PIC {

		
public:
	PIC(const PIC &copy) = delete; // prevent copying
	PIC& operator=(const PIC&) = delete; // prevent assignment
	PIC() {}
/* Add your code here */ 
	enum {
    timer    = 0,
    keyboard = 1
	};

	void allow (int interrupt_device);
	
	void forbid (int interrupt_device);
	
	bool is_masked (int interrupt_device);
};

#endif
