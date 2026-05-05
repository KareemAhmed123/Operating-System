/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            K E Y B O A R D                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Keyboard driver.                                                          */
/*****************************************************************************/

/* Add your code here */ 
#include "device/keyboard.h"

extern Plugbox plugbox;
extern PIC pic;
extern CGA_Stream kout;

/* Add your code here */ 
void Keyboard::plugin(){
	plugbox.assign(Plugbox::keyboard, *this);
	pic.allow(PIC::keyboard);
}

void Keyboard::trigger(){
	
	Key k = key_hit();
	//throw out invalid
	if (k.valid() == 0){
		return;
	}
	// Ctrl + Alt + (a)"Del" → reboot
    if ((k.ctrl() && k.alt()) && k.scancode() == /*0x1E*/ 0x53) {
		kout << "REBOOT"<<endl;
		reboot();
        return;
    }
   //Arrow Left
    if (k.scancode() == 0x4B){
		int x,y;
		kout.getpos(x,y);
		kout.setpos((x-1),y);
		return;
	}
	//Arrow Right
    if (k.scancode() == 0x4D){
		int x,y;
		kout.getpos(x,y);
		kout.setpos((x+1),y);
		return;
	}
	//Arrow Up
    if (k.scancode() == 0x48){
		int x,y;
		kout.getpos(x,y);
		kout.setpos(x,(y-1));
		return;
	}
	//Arrow Down
    if (k.scancode() == 0x50){
		int x,y;
		kout.getpos(x,y);
		kout.setpos(x,(y+1));
		return;
	}
	//retreve ASCII  
    if (k.ascii() != 0) {
        kout <<(char)k.ascii();
        kout.flush();
        return;
    }

	return;
}
 
