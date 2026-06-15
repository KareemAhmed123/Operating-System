/* Add your code here */ 
#include "device/cgastr.h"
#include "device/keyboard.h"
#include "machine/plugbox.h"
#include "machine/pic.h"
#include "user/appl.h"
#include "user/loop.h"
#include "machine/cpu.h"
#include "guard/guard.h"
#include "thread/dispatch.h"
#include "guard/secure.h"
#include "thread/scheduler.h"
#include "thread/coroutine.h"


CGA_Stream kout;
Plugbox plugbox;
PIC pic;
CPU cpu;
Guard guard;
Scheduler scheduler;


static char app_stack_1[4096];
static char app_stack_2[4096];

int id = 0;

int main()
{	//initialaization
	Keyboard keyboard;
	keyboard.plugin();
	kout.clear();	
	cpu.enable_int();
	
	Application app_1(app_stack_1 + sizeof(app_stack_1));
    scheduler.ready(app_1);
	Loop app_2(app_stack_2 + sizeof(app_stack_2));
	scheduler.ready(app_2);
	
	
    scheduler.schedule();

	
/* Add your code here */ 
	kout << "Error";
/* Add your code here */ 
	return 0;
}

//make qemu  (auto-complet)
