/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            K E Y B O A R D                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Keyboard driver.                                                          */
/*****************************************************************************/

#ifndef __Keyboard_include__
#define __Keyboard_include__

#include "machine/keyctrl.h"
#include "guard/gate.h"
#include "machine/key.h"
#include "device/cgastr.h"
#include "machine/plugbox.h"
#include "machine/pic.h"

 
class Keyboard : public Gate, public Keyboard_Controller
/* Add your code here */ 
{
protected:
	static const int SIZE = 8; //Buffer SIZE
	Key keys[SIZE];
	int posi = 0; 	//Possition of the last Empty Buffer Element	
	int poso = 0; 	//Possition of the next to handel Buffer Element
	
public:
	Keyboard(const Keyboard &copy) = delete; // prevent copying
	Keyboard& operator=(const Keyboard&) = delete; // prevent assignment
/* Add your code here */ 
	Keyboard(){}
/* Add your code here */ 
 
	// PLUGIN: "Plugs in" the keyboard (driver). From now on, keypresses are handled.
	void plugin();

/* Add your code here */
	bool prologue();
	
	void epilogue();


};

#endif
