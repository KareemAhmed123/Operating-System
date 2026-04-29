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

#include "object/o_stream.h"

/* Add your code here */ 

O_Stream::O_Stream() : Stringbuffer() {
    // Numbers are printed as decimal values by default,
    // just like in the normal C++ output stream.
    base = 10;
}

void O_Stream::set_base(int new_base) {
    // Only the number systems used by our manipulators are accepted.
    // If somebody passes a different value, we keep the old base.
    if (new_base == 2 || new_base == 8 || new_base == 10 || new_base == 16) {
        base = new_base;
    }
}

void O_Stream::print_unsigned(unsigned long long value) {
    char digits[] = "0123456789abcdef";
    char number[65];
    int index = 0;

    // The number 0 is a special case:
    // the loop below would not write any digit for it.
    if (value == 0) {
        put('0');
        return;
    }

    // We get the digits from right to left.
    // Example in decimal:
    // 123 % 10 = 3, then 12 % 10 = 2, then 1 % 10 = 1.
    while (value > 0) {
        number[index] = digits[value % base];
        value = value / base;
        index++;
    }

    // The temporary array now contains the digits backwards.
    // Therefore we print it from the last filled position down to zero.
    while (index > 0) {
        index--;
        put(number[index]);
    }
}

void O_Stream::print_signed(long long value) {
    // A minus sign only belongs to decimal output.
    // In binary, octal and hexadecimal the bit pattern is shown instead.
    if (base == 10 && value < 0) {
        put('-');
        print_unsigned(0 - (unsigned long long)value);
    } else {
        print_unsigned((unsigned long long)value);
    }
}

O_Stream& O_Stream::operator<< (char c) {
    put(c);
    return *this;
}

O_Stream& O_Stream::operator<< (unsigned char c) {
    put(c);
    return *this;
}

O_Stream& O_Stream::operator<< (char* text) {
    // Strings end with a zero byte.
    // We copy every character before that terminator into the buffer.
    if (text != 0) {
        while (*text != '\0') {
            put(*text);
            text++;
        }
    }

    return *this;
}

O_Stream& O_Stream::operator<< (const char* text) {
    if (text != 0) {
        while (*text != '\0') {
            put(*text);
            text++;
        }
    }

    return *this;
}

O_Stream& O_Stream::operator<< (short value) {
    print_signed(value);
    return *this;
}

O_Stream& O_Stream::operator<< (unsigned short value) {
    print_unsigned(value);
    return *this;
}

O_Stream& O_Stream::operator<< (int value) {
    print_signed(value);
    return *this;
}

O_Stream& O_Stream::operator<< (unsigned int value) {
    print_unsigned(value);
    return *this;
}

O_Stream& O_Stream::operator<< (long value) {
    print_signed(value);
    return *this;
}

O_Stream& O_Stream::operator<< (unsigned long value) {
    print_unsigned(value);
    return *this;
}

O_Stream& O_Stream::operator<< (long long value) {
    print_signed(value);
    return *this;
}

O_Stream& O_Stream::operator<< (unsigned long long value) {
    print_unsigned(value);
    return *this;
}

/* Pointer */

O_Stream& O_Stream::operator<<(void* pointer) {
	int old_base = base;

    *this << "0x";   // 👈 hinzufügen
    base = 16;

    print_unsigned((unsigned long)pointer);

    base = old_base;
    return *this;
}

O_Stream& O_Stream::operator<< (O_Stream& (*manipulator)(O_Stream&)) {
    return manipulator(*this);
}

O_Stream& endl(O_Stream& stream) {
    stream << '\n';
    stream.flush();
    return stream;
}

O_Stream& bin(O_Stream& stream) {
    stream.set_base(2);
    return stream;
}

O_Stream& oct(O_Stream& stream) {
    stream.set_base(8);
    return stream;
}

O_Stream& dec(O_Stream& stream) {
    stream.set_base(10);
    return stream;
}

O_Stream& hex(O_Stream& stream) {
    stream.set_base(16);
    return stream;
}
