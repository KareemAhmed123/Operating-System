/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            O _ S T R E A M                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The O_Stream class defines the << operator for several pre-defined data   */
/* types and thereby realizes output functionality similar to C++'s iostream */
/* library. By default, this class supports printing characters, strings and */
/* integer numbers of various bit sizes. Another << operator allows to use   */
/* so-called 'manipulators'.                                                 */
/*                                                                           */
/* Besides class O_Stream this file also defines the manipulators hex, dec,  */
/* oct and bin for choosing the basis in number representations, and endl    */
/* for implementing an (implicitly flushing) line termination.               */
/*****************************************************************************/

#ifndef __o_stream_include__
#define __o_stream_include__

#include "object/strbuf.h"

class O_Stream
/* Add your code here */ 
	: public Stringbuffer
{
private:
	int base;

	void print_unsigned(unsigned long long value);
	void print_signed(long long value);

public:
	O_Stream(const O_Stream &copy) = delete; // prevent copying
	O_Stream& operator=(const O_Stream&) = delete; // prevent assignment
/* Add your code here */ 
	O_Stream();

	void set_base(int new_base);

	O_Stream& operator<< (char c);
	O_Stream& operator<< (unsigned char c);
	O_Stream& operator<< (char* text);
	O_Stream& operator<< (const char* text);
	O_Stream& operator<< (short value);
	O_Stream& operator<< (unsigned short value);
	O_Stream& operator<< (int value);
	O_Stream& operator<< (unsigned int value);
	O_Stream& operator<< (long value);
	O_Stream& operator<< (unsigned long value);
	O_Stream& operator<< (long long value);
	O_Stream& operator<< (unsigned long long value);
	O_Stream& operator<< (O_Stream& (*manipulator)(O_Stream&));
};

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          M A N I P U L A T O R S                          */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The functions below all receive and return a reference to an O_Stream     */
/* object. Because class O_Stream defines an operator<< for functions with   */
/* this signature, they can be called in this operator's implementation and  */
/* even be embedded in a "stream" of outputs. A manipulator's task is to     */
/* influence subsequent output, e.g. by switching to a different number      */
/* system.                                                                   */
/*---------------------------------------------------------------------------*/

// ENDL: inserts a newline in the output and flushes the buffer
/* Add your code here */ 
O_Stream& endl(O_Stream& stream);

// BIN: selects the binary number system
/* Add your code here */ 
O_Stream& bin(O_Stream& stream);

// OCT: selects the octal number system
/* Add your code here */ 
O_Stream& oct(O_Stream& stream);

// DEC: selects the decimal number system
/* Add your code here */ 
O_Stream& dec(O_Stream& stream);

// HEX: selects the hexadecimal number system
/* Add your code here */ 
O_Stream& hex(O_Stream& stream);

#endif
