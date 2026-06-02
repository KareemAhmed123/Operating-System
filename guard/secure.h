#ifndef __Secure_include__
#define __Secure_include__

#include "guard/guard.h"

extern Guard guard;

class Secure{
public:
    Secure(){
        guard.enter();
    }
    ~Secure(){
        guard.leave();
    }


};
#endif