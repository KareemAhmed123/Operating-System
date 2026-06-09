/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 T O C                                     */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The toc struct is used to save the values of the non-volatile registers   */
/* in case of a context switch. toc_settle prepares the initial stack and    */
/* the toc struct for the first activation.                                  */
/*****************************************************************************/

#include "machine/toc.h"

// TOC_SETTLE: Prepares a coroutine context for its first activation.
void toc_settle(struct toc *regs, void *tos,
		void (*kickoff)(void *, void *, void *, void *, void *, void *,
				void *),
		void *object)
{
/* Add your code here */ 
    void **stack = (void **)tos;

    // Stack grows down
    *(--stack) = object;          // 7. Parameter for kickoff
    *(--stack) = 0;               // Padding / fake slot
    *(--stack) = (void*)kickoff;  //  return address

    regs->rbx = 0;  // remember starting Function
    regs->r12 = 0;
    regs->r13 = 0;
    regs->r14 = 0;
    regs->r15 = 0;
    regs->rbp = 0;
    regs->rsp = stack;
}
