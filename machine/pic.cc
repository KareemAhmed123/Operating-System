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

/* Add your code here 
	Bit = 1 → Interrupt BLOCKIERT
	Bit = 0 → Interrupt ERLAUBT
 */ 
#include "machine/pic.h"

void PIC::allow (int interrupt_device){ //activates Interupt for a device
	unsigned char mask = inb(0x21);
	mask &= ~(1 << interrupt_device);
	outb(0x21, mask);
}

void PIC::forbid (int interrupt_device){ //deactivates Interupt for a device
	unsigned char mask = inb(0x21);
	mask |= (1 << interrupt_device);
	outb(0x21, mask);
}

bool PIC::is_masked (int interrupt_device){ //returns if interrupts of this device are not forwarded to the CPU, return true, otherwise return false.
	unsigned char mask = inb(0x21);
	return mask & (1 << interrupt_device);
}
