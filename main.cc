/* Add your code here */ 
#include "device/cgastr.h"

// This global stream object is the test output device.
// It combines O_Stream's << operators with CGA_Screen's screen access.
CGA_Stream kout;

int main()
{
/* Add your code here */ 
 /*
  * Old CGA_Screen-only test code:
  *
  * #include "machine/cgascr.h"
  *
  * CGA_Screen screen;
  * screen.show(0, 1, 'h', 0x08);
  * screen.show(1, 1, 'i', 0x07);
  *
  * screen.setpos(0, 2);
  * char text[] = "Hello other Gen";
  * screen.print(text, sizeof(text) - 1, 0x7);
  *
  * What this old test did:
  * - show() wrote single characters directly to fixed screen positions.
  * - setpos() moved the hardware cursor manually.
  * - print() wrote a raw character array with a fixed color attribute.
  *
  * That was useful for testing CGA_Screen, but it did not test O_Stream.
  */

 // Start the stream test at the top-left corner of the screen.
 kout.setpos(0, 0);

 // endl writes a newline and flushes the stream buffer to the CGA screen.
 kout << "O_Stream test" << endl;
 kout << "-------------" << endl;

 // Test character and string output.
 kout << "char: " << 'A' << endl;
 kout << "string: " << "Hello from CGA_Stream" << endl;

 // Test signed and unsigned decimal output.
 kout << "signed decimal: " << -42 << endl;
 kout << "unsigned decimal: " << 12345u << endl;

 // Test the base manipulators.
 // Each manipulator changes how the following integer numbers are printed.
 kout << "hex 255: " << hex << 255 << endl;
 kout << "oct 255: " << oct << 255 << endl;
 kout << "bin 13: " << bin << 13 << endl;

 // Switch back to decimal so later numbers are readable in the normal base.
 kout << dec << "back to dec: " << 255 << endl;

 // Show that several bases can be used in one output chain.
 kout << "mixed: " << dec << 10 << " " << hex << 10 << " " << bin << 10 << endl;


/* Add your code here */ 
 
/* Add your code here */ 
 
	return 0;
}
