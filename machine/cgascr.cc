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

CGA_Screen::CGA_Screen() : index_port(INDEX_PORT), data_port(DATA_PORT) {
    // When the screen object is created, we start at the top-left corner.
    // Top-left means column 0 and row 0.
    cursor_x = 0;
    cursor_y = 0;
    
}

void CGA_Screen::show (int x, int y, char c, unsigned char attrib){
    // The screen looks like rows and columns to us.
    // But in memory it is stored as one long line.
    //
    // To find a character place:
    // 1. Skip all complete rows before row y.
    //    That is: y * WIDTH.
    // 2. Move x columns into that row.
    //    That is: + x.
    //
    // Example:
    // If WIDTH is 80 and we want x = 5, y = 2:
    // y * WIDTH + x = 2 * 80 + 5 = 165.
    // So this is character place number 165.
    //
    // Each character place uses 2 bytes in memory:
    // first byte  = the character itself
    // second byte = the color/style
    pos = CGA_START + 2 * (x + y * WIDTH);
    *pos = c;
    *(pos+1) = attrib;
}

void CGA_Screen::setpos (int x, int y){
    // Save the cursor position in our own variables.
    // This makes it easy for other methods, like print(), to know where we are.
    cursor_x = x;
    cursor_y = y;

    // The hardware cursor does not understand "x and y".
    // It only understands one number: the character place on the screen.
    //
    // So we convert x/y into one number.
    // Example:
    // x = 5, y = 2, WIDTH = 80
    // position = 2 * 80 + 5 = 165
    unsigned short position = y * WIDTH + x;

    // The position number is too large to send in one small hardware write.
    // So we split it into two parts:
    // high byte = upper part of the number
    // low byte  = lower part of the number
    index_port.outb(14);                 // select cursor high byte register
    data_port.outb(position >> 8);        // send high byte

    index_port.outb(15);                 // select cursor low byte register
    data_port.outb(position & 0xFF);      // send low byte
}

void CGA_Screen::getpos (int &x, int &y){
    // This function asks the hardware:
    // "Where is the blinking cursor right now?"
    //
    // The hardware answers with one position number.
    // Because that number is split into two parts, we read both parts.
    index_port.outb(14);                 // select cursor high byte register
    unsigned short position = data_port.inb() << 8;

    index_port.outb(15);                 // select cursor low byte register
    position |= data_port.inb();

    // Now we convert the one position number back into x and y.
    //
    // x is the column.
    // The modulo operator (%) gives the remainder after dividing by WIDTH.
    // That remainder tells us how far into the row the cursor is.
    //
    // y is the row.
    // Dividing by WIDTH tells us how many full rows come before the cursor.
    //
    // Example:
    // position = 165, WIDTH = 80
    // x = 165 % 80 = 5
    // y = 165 / 80 = 2
    cursor_x = position % WIDTH;
    cursor_y = position / WIDTH;

    // Give the result back to the caller.
    // Because x and y are references, changing them here changes the
    // variables that were passed into this function.
    x = cursor_x;
    y = cursor_y;
}

void CGA_Screen::print (char* text, int length, unsigned char attrib){
    for(int i=0; i<length; i++){
        // Print the current character at the current cursor position.
        show(cursor_x, cursor_y, text[i], attrib);

        // After printing one character, move the cursor one column right.
        cursor_x++;
        
        // If the cursor moved past the last column,
        // go to the first column of the next row.
        //
        // Example:
        // If cursor_x becomes 80, that is outside the row.
        // So we set cursor_x back to 0 and increase cursor_y.
        if(cursor_x >= WIDTH){
            cursor_x = 0;
            cursor_y++;
        }
    }

    // The loop above changes cursor_x and cursor_y in our program.
    // Now we also move the real blinking hardware cursor to the same place.
    setpos(cursor_x, cursor_y);
}
