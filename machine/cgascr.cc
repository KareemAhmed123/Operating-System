/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                             C G A _ S C R E E N                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* This class allows accessing the PC's screen.  Accesses work directly on   */
/* the hardware level, i.e. via the video memory and the graphic adapter's   */
/* I/O ports.                                                                */
/*****************************************************************************/

#include "machine/cgascr.h"

/* Add your code here */ 
CGA_Screen::CGA_Screen(){
	cursor_x = 0;
	cursor_y = 0;
	
}

void CGA_Screen::show (int x, int y, char c, unsigned char attrib){
		pos = CGA_START + 2 * ( x + y*80);
		*pos = c;
		*(pos+1) = attrib;
} 

void CGA_Screen::setpos (int x, int y){
	cursor_x = x;
	cursor_y = y;
	update_cursor(cursor_x, cursor_y);
}

void CGA_Screen::getpos (int &x, int &y){
	x = cursor_x;
	y = cursor_y;
}

void CGA_Screen::print (char* text, int length, unsigned char attrib){
	for(int i=0; i<length; i++){
		
		if (text[i] == '\n') {
			cursor_x = 0;
			cursor_y++;
			continue;
		}
		
		show(cursor_x, cursor_y, text[i], attrib);
		cursor_x++;
		
		if(cursor_x >=80){
			cursor_x = 0;
			cursor_y++;
		}
	}
	update_cursor(cursor_x, cursor_y);
}

void CGA_Screen::update_cursor(int x, int y)
{
	int pos = y * 80 + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (int) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (int) ((pos >> 8) & 0xFF));
}

void CGA_Screen::clear(){
	char l = ' ';
	for(int i = 0; i < 25; i++){
		for(int j = 0; j<80;j++){
			show(j,i,l,0x7);
		}
	}
	int o = 0;
	setpos(o,o);
}

void CGA_Screen::scrol(bool direction){
	// needs to be implemented
}
