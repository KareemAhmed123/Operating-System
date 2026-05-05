/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 P A N I C                                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Default interrupt handler.                                                */
/*****************************************************************************/

#ifndef __panic_include__
#define __panic_include__

/* INCLUDES */

#include "guard/gate.h"
#include "device/cgastr.h"
#include "machine/cpu.h"

class Panic : public Gate
/* Add your code here */ 
{
public:
	Panic (const Panic &copy) = delete; // prevent copying
	Panic& operator=(const Panic&) = delete; // prevent assignment
	Panic () {}
/* Add your code here */ 
	void trigger();
};

#endif
