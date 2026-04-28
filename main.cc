/* Add your code here */ 
#include "machine/cgascr.h"
int main()
{
/* Add your code here */ 
 CGA_Screen screen;
 screen.show(0,1,'h',0x08);
 screen.show(1,1,'i',0x07);

 screen.setpos(0,2);
 char text[]="Hello other Gen";
 screen.print(text,sizeof(text)-1,0x7);


/* Add your code here */ 
 
/* Add your code here */ 
 
	return 0;
}
