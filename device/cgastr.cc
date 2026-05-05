/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         C G A _ S T R E A M                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The CGA_Stream class allows to print different data types as text strings */
/* to a PC's CGA screen.                                                     */
/* For attributes/colors and cursor positioning use the methods of class     */
/* CGA_Screen.                                                               */
/*****************************************************************************/

#include "device/cgastr.h"

/* Add your code here */ 

CGA_Stream::CGA_Stream() : O_Stream(), CGA_Screen() {
}

void CGA_Stream::flush() {
    // Buffer ausgeben
    print(Stringbuffer::buf, Stringbuffer::pos, 0x07);

    // Buffer zurücksetzen
   Stringbuffer::pos = 0;
}
