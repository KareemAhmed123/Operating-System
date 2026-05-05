/* Add your code here */ 
#include "device/cgastr.h"
#include "device/keyboard.h"
#include "machine/plugbox.h"
#include "machine/pic.h"
#include "user/appl.h"
#include "machine/cpu.h"

//Test
//#include "machine/keyctrl.h"
Keyboard_Controller keyb;


CGA_Stream kout;
Plugbox plugbox;
PIC pic;
CPU cpu;

int main()
{	//initialaization
	Keyboard keyboard;
	keyboard.plugin();
	kout.clear();
	
	//interupt enabaling
	//cpu.enable_int();
	//pic.allow(PIC::keyboard);
	
	//Aplication
	
	//Selection of Program
	kout << "Choose Program:" << endl;
	kout << "1 Mess" << endl;
	kout << "2 Fix" << endl;
	bool c = true;
	int i;
	while(c){
		Key k = keyb.key_hit();
		if (k.valid()) { 
			if(k.scancode() == 0x2){
				i = 1;
				c = false;
				kout << 1;
			} else if(k.scancode() == 0x3){
				i = 2;
				c = false;
				kout << 2;
			}
		}
	}
	kout.clear();
	for(int a = 0; a<100; a++){}
	cpu.enable_int();
	pic.allow(PIC::keyboard);
	
	//running Program
	Application application;
	while (1) {application.action(i);} 
/* Add your code here */ 
	kout << "Error";
/* Add your code here */ 
	return 0;
}

//make qemu  (auto-complet)
