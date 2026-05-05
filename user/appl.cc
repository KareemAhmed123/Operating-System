/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Application class defines the (only) application for OOStuBS.         */
/*****************************************************************************/

/* INCLUDES */

#include "user/appl.h"
#include "device/cgastr.h"
#include "machine/cpu.h"
/* Add your code here */ 
 
/* GLOBAL VARIABLES */

extern CGA_Stream kout;
extern CPU cpu;
/* Add your code here */ 
 
void Application::action(int m)
{
/* Add your code here */ 
/* prints a and later A at the same Position
	int m is for chosing the Mode:
	m=2 => interupts are disabeld for the output step
*/
	int x = 40;
	int y = 5;
	int dx , dy ;
	if(m==2){cpu.disable_int();}
	kout.getpos (dx,dy);
	kout.setpos(x,y);
	kout << "a";
	kout.flush();
	kout.setpos(dx,dy);
	if(m==2){cpu.enable_int();}
	for(int i=0; i<2000000; i++){}
	if(m==2){cpu.disable_int();}
	kout.getpos (dx,dy);
	kout.setpos(x,y);
	kout << "A";
	kout.flush();
	kout.setpos(dx,dy);
	if(m==2){cpu.enable_int();}
	for(int i=0; i<2000000; i++){}
 
}
