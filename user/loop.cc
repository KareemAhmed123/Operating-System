/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 L O O P                                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Loop is a thread that does nothing else but count upwards and print this  */
/* on the screen. In between, it yields the CPU. The Scheduler then decides  */
/* which thread shall run next.                                              */
/*****************************************************************************/

/* Add your code here */ 
#include "user/loop.h"

extern CGA_Stream kout;
extern Guard guard;
extern Scheduler scheduler; 

extern int id;
/* Add your code here */ 

 
void Loop::action(){	
	//kout << "A";
	//kout.flush();
	int y;
	int z = 0; 
	int x = 40;
	int dx , dy ;
	y = id;
	id++;
	//if (id < 5){id++;}
	//kout << "B";
	//kout.flush();

	while (1){
	//	kout << "C";
	//	kout.flush();
	//	kout << y ;
	//	kout.flush();
		while (z<10){
			guard.enter();
			kout.getpos (dx,dy);
			kout.setpos(x,y);
			kout << y << " " << z;
			kout.flush();
			kout.setpos(dx,dy);
			guard.leave();
			z++;
		}
	//	kout << endl;
		z=0;
		if (y == 1){
			scheduler.exit();
		}
		for(int i=0;i<100000;i++){}//so interupts still hav a chance
		scheduler.resume();
	//	kout << "H";
	//	kout.flush();
	}
}
