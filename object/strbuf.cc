/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                       S T R I N G B U F F E R                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Stringbuffer class provides a buffer for collecting characters to be  */
/* printed to an output device, in our case the PC screen. The actual output */
/* occurs once the buffer is full, or when the user explicitly calls         */
/* flush().                                                                  */
/* As Stringbuffer is intended to be device independent, flush() is a        */
/* (pure) virtual method that must be defined by derived classes.            */
/*****************************************************************************/

// This includes the header file that contains the class declaration.
// In Java terms, this is where the compiler learns:
// - what Stringbuffer is
// - what fields it has
// - what methods it declares
#include "object/strbuf.h"

// Constructor implementation for class Stringbuffer.
// This runs when a Stringbuffer-derived object is created.
Stringbuffer::Stringbuffer() {
    // Initialize the current buffer position to 0.
    // That means: the buffer is empty at the beginning.
    //
    // pos tells us where the next character should be placed.
    // If pos == 0, the next character will go into buf[0].
    pos = 0;
}

// This method puts one character into the buffer.
void Stringbuffer::put(char c) {
    // Store the given character c into the current free position.
    // Example:
    // if pos == 0 and c == 'A', then buf[0] = 'A'
    buf[pos] = c;

    // Move pos to the next free position.
    // After writing one character, the next character must go to the next slot.
    pos++;

    // Check whether the buffer is now full.
    //
    // SIZE is the maximum number of characters the buffer can hold.
    // If pos >= SIZE, there is no more free place left.
    if (pos >= SIZE) {
        // flush() sends the collected characters to the real output device.
        //
        // Important:
        // Stringbuffer itself does NOT know how to output to the screen.
        // It only knows how to collect characters.
        //
        // The actual output behavior is implemented in a derived class.
        flush();

        // After flushing, the buffer is considered empty again.
        // So we reset pos to 0 and start filling from the beginning.
        pos = 0;
    }
}