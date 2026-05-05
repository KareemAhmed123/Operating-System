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

O_Stream::O_Stream() : Stringbuffer(), base(10) {
}

void O_Stream::set_base(int b) {
    base = b;
}

/* Hilfsfunktion für Zahlenausgabe */
void O_Stream::print_number(unsigned long number) {
    char tmp[32];
    int i = 0;

    if (number == 0) {
        put('0');
        return;
    }

    while (number > 0) {
        unsigned long digit = number % base;

        if (digit < 10)
            tmp[i++] = '0' + digit;
        else
            tmp[i++] = 'A' + (digit - 10);

        number /= base;
    }

    while (i > 0) {
        put(tmp[--i]);
    }
}

/* Zeichen */

O_Stream& O_Stream::operator<<(char c) {
    put(c);
    return *this;
}

O_Stream& O_Stream::operator<<(unsigned char c) {
    put((char)c);
    return *this;
}

/* String */

O_Stream& O_Stream::operator<<(char* text) {
    while (*text) {
        put(*text++);
    }
    return *this;
}

/* Zahlen */

O_Stream& O_Stream::operator<<(unsigned short number) {
    print_number(number);
    return *this;
}

O_Stream& O_Stream::operator<<(short number) {
    if (base == 10 && number < 0) {
        put('-');
        print_number(-number);
    } else {
        print_number((unsigned short)number);
    }
    return *this;
}

O_Stream& O_Stream::operator<<(unsigned int number) {
    print_number(number);
    return *this;
}

O_Stream& O_Stream::operator<<(int number) {
    if (base == 10 && number < 0) {
        put('-');
        print_number(-number);
    } else {
        print_number((unsigned int)number);
    }
    return *this;
}

O_Stream& O_Stream::operator<<(unsigned long number) {
    print_number(number);
    return *this;
}

O_Stream& O_Stream::operator<<(long number) {
    if (base == 10 && number < 0) {
        put('-');
        print_number(-number);
    } else {
        print_number((unsigned long)number);
    }
    return *this;
}

/* Pointer */

O_Stream& O_Stream::operator<<(void* pointer) {
	int old_base = base;

    *this << "0x";   // 👈 hinzufügen
    base = 16;

    print_number((unsigned long)pointer);

    base = old_base;
    return *this;
}

/* Manipulator-Aufruf */

O_Stream& O_Stream::operator<<(O_Stream& (*f)(O_Stream&)) {
    return f(*this);
}

/* ===================== */
/*     MANIPULATORS      */
/* ===================== */

O_Stream& endl(O_Stream& os) {
    os << '\n';
    os.flush();
    return os;
}

O_Stream& bin(O_Stream& os) {
    os.set_base(2);
    return os;
}

O_Stream& oct(O_Stream& os) {
    os.set_base(8);
    return os;
}

O_Stream& dec(O_Stream& os) {
    os.set_base(10);
    return os;
}

O_Stream& hex(O_Stream& os) {
    os.set_base(16);
    return os;
}
