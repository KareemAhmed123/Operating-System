#ifndef __screen_include__
#define __screen_include__

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

#include "machine/io_port.h"

class CGA_Screen {
private:
/* Add your code here */ 
	// The screen is treated like a table.
	// One row has 80 character spaces.
	// Example: x = 0 is the first column, x = 79 is the last column.
	static const int WIDTH = 80;

	// These two numbers are hardware addresses.
	// We use them when we want to move or read the blinking text cursor.
	//
	// INDEX_PORT: choose what cursor information we want to access.
	// DATA_PORT:  send or receive the actual value.
	static const int INDEX_PORT = 0x3d4;
	static const int DATA_PORT = 0x3d5;

	// This is where the text screen starts in memory.
	// If we write characters to this memory, they appear on the screen.
	char* CGA_START=(char *)0xb8000;

	// This points to the exact place in screen memory we are writing to.
	char* pos;

	// These objects let the C++ code talk to the hardware ports above.
	IO_Port index_port;
	IO_Port data_port;

	// These remember the cursor position in a simple form.
	// cursor_x is the column.
	// cursor_y is the row.
	int cursor_x;
	int cursor_y;

public:
	CGA_Screen(const CGA_Screen &copy) = delete; // prevent copying
	CGA_Screen& operator=(const CGA_Screen&) = delete; // prevent assignment
	CGA_Screen();
/* Add your code here */ 
	// Show one character on the screen.
	// x and y say where to put it.
	// attrib says the color/style.
	void show (int x, int y, char c, unsigned char attrib);

	// Move the blinking cursor to a new screen position.
	void setpos (int x, int y);

	// Read the current blinking cursor position from the hardware.
	// The result is written into x and y.
	void getpos (int&x, int& y);

	// Print a whole text, one character after another.
	void print (char* text, int length, unsigned char attrib);

/* Add your code here */ 
};

/* Add your code here */ 

#endif
