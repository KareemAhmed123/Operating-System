/* Add your code here */ 
#include "device/cgastr.h"
#include "machine/keyctrl.h"

// This global stream object is the test output device.
// It combines O_Stream's << operators with CGA_Screen's screen access.
CGA_Stream kout;

int main()
{
/* Add your code here */ 
	
	Keyboard_Controller keyboard;
	
    
    /*Test : CGA_Stream */
    
    /*kout << "Test          <stream result> -> <expected>" << endl;
	kout << "zero:         " << 0 << " -> 0" << endl;
	kout << "decimal:      " << dec << 42 << " -> 42" << endl;
	kout << "binary:       " << bin << 42 << dec << " -> 0b101010" << endl;
	kout << "octal:        " << oct << 42 << dec << " -> 052" << endl;
	kout << "hex:          " << hex << 42 << dec << " -> 0x2a" << endl;
	kout << "uint64_t max: " << ~((unsigned long)0) << " -> 18446744073709551615" << endl;
	kout << "int64_t max:  " << ~(1l<<63) << " -> 9223372036854775807" << endl;
	kout << "int64_t min:  " << (1l<<63) << " -> -9223372036854775808" << endl;
	kout << "some int64_t: " << (-1234567890123456789) << " -> -1234567890123456789" << endl;
	kout << "some int64_t: " << (1234567890123456789) << " -> 1234567890123456789" << endl;
	kout << "pointer:      " << reinterpret_cast<void*>(1994473406541717165ul) << " -> 0x1badcafefee1dead" << endl;
	kout << "smiley:       " << static_cast<char>(1) << endl;*/
    
    

    /*Test : Keyctr */ 
	kout << "Press keys..." << endl;

	int s =0;
	int d = 0; 
	int z = 0;
	
    while (1) {
        Key k = keyboard.key_hit();
        if (k.valid()) {
			if (k.scancode()== 0x1){
				kout << endl <<"d for Delay"  << "s for speed" << endl;
				while ( z == 0){
					Key seter = keyboard.key_hit();
					char set = (char)seter.ascii();
					if (set == 'd'){
						d++;
						if (d > 3){ d=0;}
						
					} else if (set == 's'){
						s = s + 5;
						if (s>31){s=0;}						
					} else if (seter.scancode() == 0x1){
						z = 1;
					}
				}
				z = 0;
				keyboard.set_repeat_rate(s, d);
				kout <<"Delay: " << d << " Speed: " << s << endl;
			} else {
				kout << k.ascii();
			}
        }
        kout.flush();
    }


/* Add your code here */ 
 
/* Add your code here */ 
 
	return 0;
}
