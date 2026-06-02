#ifndef __Queue_include__
#define __Queue_include__
#include "object/chain.h"
class Queue {
    private:
    //Chain* means: pointer to a Chain object.
        Chain* head;
    //Chain** means: pointer to a pointer to a Chain
        Chain** tail;
        
    public:
        Queue();
        void enqueue(Chain* item);
        Chain* dequeue();
        void remove(Chain* item);
};

#endif