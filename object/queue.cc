#include "object/queue.h"

/*
 * Create an empty queue.
 *
 * head points to the first element.
 * tail points to the place where the next element should be inserted.
 *
 * In an empty queue, the next insertion place is head itself.
 */
Queue::Queue()
{
    head = 0;
    tail = &head;
}

/*
 * Insert item at the end of the queue.
 */
void Queue::enqueue(Chain* item)
{
    /*
     * The new item becomes the last element,
     * so it must not point to another element.
     */
    item->next = 0;

    /*
     * tail points to the pointer where the new item belongs.
     *
     * Empty queue:
     *   tail == &head
     *   *tail = item  means  head = item
     *
     * Non-empty queue:
     *   tail == &(last_element->next)
     *   *tail = item  means  last_element->next = item
     */
    *tail = item;

    /*
     * The new insertion place is now the next pointer of this item.
     */
    tail = &item->next;
}

/*
 * Remove and return the first element of the queue.
 *
 * If the queue is empty, return 0.
 */
Chain* Queue::dequeue()
{
    Chain* item = head;

    if (item == 0) {
        return 0;
    }

    /*
     * The second element becomes the new first element.
     */
    head = item->next;

    /*
     * If there was no second element, the queue is now empty.
     * Therefore the next insertion place is head again.
     */
    if (head == 0) {
        tail = &head;
    }

    /*
     * Detach the returned item from the queue.
     */
    item->next = 0;
    return item;
}

/*
 * Remove a specific item from the queue.
 *
 * current is a pointer to the pointer that currently leads to
 * the element we are looking at.
 *
 * At the beginning:
 *   current == &head
 *
 * Later:
 *   current == &(some_element->next)
 */
void Queue::remove(Chain* item)
{
    Chain** current = &head;

    while (*current != 0) {
        if (*current == item) {
            /*
             * Bypass item.
             *
             * If current == &head:
             *   head = item->next
             *
             * If current == &(previous->next):
             *   previous->next = item->next
             */
            *current = item->next;

            /*
             * If the pointer we just changed became 0,
             * item was the last element.
             * So tail must point to this empty next-position.
             */
            if (*current == 0) {
                tail = current;
            }

            /*
             * Detach item completely.
             */
            item->next = 0;
            return;
        }

        /*
         * Move current to the address of the next pointer.
         */
        current = &((*current)->next);
    }
}