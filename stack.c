/****************************************************************************

                                                Mark Zeng
                                                CSE 12, fall 19
                                                Due date: Oct. 15th
                                                cs12fa19dm
                              Assignment Three

File Name:      stack.c
Description:    This file contains the functions on a stack including
                new_Stack, delete_Stack, pop, top, push, isempty_Stack, 
		isfull_Stack, get_occupancy and write.
****************************************************************************/
#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "stack.h"

#define STACK_POINTER_INDEX (-1)        /* Index of last used space */
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */
#define EMPTY_POINTER (-1)             /* Index of an empty stack */ 

/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";
static const char INCOMING_NONEXIST[] = 
                        "Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] = 
                        "Isempty check from a non-existent stack!!!\n";
static const char ISFULL_NONEXIST[] = 
                        "Isfull check from a non-existent stack!!!\n";
static const char NUM_NONEXIST[] = 
                        "get_occupancy check from a non-existent stack!!!\n";
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n"; 
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";
static const char WRITE_NONEXIST_FILE[] = 
                        "Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] = 
                        "Attempt to write to a non-existent stack!!!\n";

/* Debug messages. HEX messages used for negative numbers on the stack. */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";
static const char TOP[] = "[Stack %ld - Topping %ld]\n";

/* static variable allocation */
static int debug = FALSE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */

/* Debug state methods */
void debug_off (void) {
        debug = FALSE;
}

void debug_on (void) {
        debug = TRUE;
}
 
/*---------------------------------------------------------------------------
Function Name:                delete_Stack
Purpose:                      delete the stack
Description:                  This destructor function deallocates all memory 
                              associated with the stack and sets its pointer 
                              in the calling function to NULL.
Input:                        pointer to the stack
Output:                       nothing
Result:                       stack deleted
Side Effects:                 None.
---------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {
    /*check for pointer*/
    if(spp == NULL)
    fprintf(stderr, DELETE_NONEXIST);
    
    /*check for pointer of the pointer*/
    else if(*spp == NULL)
    fprintf(stderr, DELETE_NONEXIST);
     
    else{
    /*deallocate memory*/
    free(*spp - STACK_OFFSET);
    
    *spp = 0;
    
    if(debug)   /*debug messege*/
    fprintf(stderr, DEALLOCATE, (long)stack_counter);
    
    stack_counter--;
    }
}

/*---------------------------------------------------------------------------
Function Name:                empty_Stack
Purpose:                      Effectively empties the stack of all its elements
Description:                  set the pointer to -1
Input:                        pointer to the stack
Output:                       nothing
Result:                       stack emptied
Side Effects:                 None.
---------------------------------------------------------------------------*/
void empty_Stack (Stack * this_Stack) {
    /*check for pointer*/
    if(this_Stack == NULL)
    fprintf(stderr, EMPTY_NONEXIST);
    
    /*empty stack*/
    else    
    this_Stack[STACK_POINTER_INDEX] = EMPTY_POINTER;
}

/*---------------------------------------------------------------------------
Function Name:                isempty_Stack
Purpose:                      check if the stack is empty
Description:                  Returns true if this_Stack is empty, and false 
                              if it is not.
Input:                        pointer to the stack
Output:                       1 or 0
Result:                       stack empty or not status returned
Side Effects:                 None.
---------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_Stack) {
    
    /*check for pointer*/
    if(this_Stack == NULL) {
	    fprintf(stderr, ISEMPTY_NONEXIST);
	    return 0;
    }
    
    /*check empty*/
    if(this_Stack[STACK_POINTER_INDEX] == EMPTY_POINTER)
	    return 1;
    else
	    return 0;
}
/*---------------------------------------------------------------------------
Function Name:                isfull_Stack
Purpose:                      check if the stack is full
Description:                  Returns true if this_Stack is full, and false 
                              if it is not.
Input:                        pointer to the stack
Output:                       1 or 0
Result:                       stack full or not status returned
Side Effects:                 None.
---------------------------------------------------------------------------*/
long isfull_Stack (Stack * this_Stack) {
    /*check for pointer*/
    if(this_Stack == NULL){
	    fprintf(stderr, ISFULL_NONEXIST);
	    return 0;
    }
    
    /*check full*/
    if(this_Stack[STACK_POINTER_INDEX] == this_Stack[STACK_SIZE_INDEX] - 1)
	    return 1;
    else 
	    return 0; 
}
/*---------------------------------------------------------------------------
Function Name:                new_Stack
Purpose:                      allocates and initializes a new Stack object
Description:                  allocates memory to hold stacksize number of 
                              longs, initializes the stack infrastructure, 
                              and returns a pointer to the first storage
                              space in the stack.
Input:                        value of stack size
Output:                       a pointer to the first storage
Result:                       stack created
Side Effects:                 None.
---------------------------------------------------------------------------*/
Stack * new_Stack (unsigned long stacksize) {
    /*allocate memory for new stack*/
    void * memory = (Stack*)malloc((stacksize + STACK_OFFSET)*sizeof(long));
    
    /* initialize a stack */
    Stack * this_Stack = (Stack*)memory + STACK_OFFSET;
    
    /*initialize pointer index as -1*/
    this_Stack[STACK_POINTER_INDEX] = EMPTY_POINTER;
    
    /*initialize stack size*/
    this_Stack[STACK_SIZE_INDEX] = stacksize - '0';
    
    /*initialize stack count*/
    stack_counter++;
    this_Stack[STACK_COUNT_INDEX] = stack_counter;
    
    if(debug)     /*debug messege*/
    fprintf(stderr, ALLOCATED, (long)stack_counter);
    
    return this_Stack;
    
}
/*---------------------------------------------------------------------------
Function Name:          get_occupancy
Purpose:                Returns the number of elements in this_Stack.
Description:            check if the parameter is a pointer to the 
                        stack and return the number of elements in this_stack
Input:                  pointer of the stack
Output:                 number of elements
Result:                 number of elements returned
Side Effects:           None.
---------------------------------------------------------------------------*/
long get_occupancy (Stack * this_Stack) {
    /*check for pointer*/
    if(this_Stack == NULL){
	    fprintf(stderr, NUM_NONEXIST);
	    return 0;
    }
    
    return this_Stack[STACK_POINTER_INDEX] + 1 ;
}
/*---------------------------------------------------------------------------
Function Name:               pop
Purpose:                     Removes an item from the top of the stack, and
                             sends it back through the output parameter item.
Description:                 Removes an item from the top of the stack, and 
                             sends it back through the output parameter item. 
                             Returns true upon success and false upon failure
Input:                       pointer to the stack and the item to be poped
Output:                      seccess or failure status 
Result:                     top item removed,seccess or failure status returned
Side Effects:                None.
---------------------------------------------------------------------------*/
long pop (Stack * this_Stack, long * item) {
    /*check for stack pointer*/
    if(this_Stack == NULL){
	    fprintf(stderr, POP_NONEXIST);
	    return 0;
    }
    
    /*check item pointer*/
    if(item == NULL){
	    fprintf(stderr, INCOMING_NONEXIST);
	    return 0;
    }
    /*check empty*/
    if(isempty_Stack(this_Stack)){
	    fprintf(stderr, POP_EMPTY);
            return 0;
    }
   
    /*pop the item*/
    *item = this_Stack[this_Stack[STACK_POINTER_INDEX]];    
    
     /*debug messege*/
    if(debug){
	    if(this_Stack[this_Stack[STACK_POINTER_INDEX]]> 0) 
            fprintf(stderr, POP, this_Stack[STACK_COUNT_INDEX], *item); 
            /*use hexpop if item in stack is negative*/
	    else
            fprintf(stderr, HEXPOP, this_Stack[STACK_COUNT_INDEX], *item); 
    }

    this_Stack[STACK_POINTER_INDEX]--;
    
 
    return 1;
     
}
/*---------------------------------------------------------------------------
Function Name:                push
Purpose:                      Adds item to the top of this_Stack.
Description:                  Adds item to the top of this_Stack. Return true
                              upon success and false upon failure.
Input:                        pointer to the stack and the item to be pushed
Output:                       push successful or not
Result:                       item added, seccess or failure status returned
Side Effects:                 None.
---------------------------------------------------------------------------*/
long push (Stack * this_Stack, long item) {    
    /*check for stack pointer*/
    if(this_Stack == NULL){
	    fprintf(stderr, PUSH_NONEXIST);
	    return 0;
    }
        
    /*check full*/
    if(isfull_Stack(this_Stack)){
	    fprintf(stderr, PUSH_FULL);
	    return 0;
    }
    
    /*push item*/
    this_Stack[STACK_POINTER_INDEX]++;
    this_Stack[this_Stack[STACK_POINTER_INDEX]] = item;
    
     /*debug messege*/
    if(debug){
	    if(this_Stack[this_Stack[STACK_POINTER_INDEX]]> 0) 
            fprintf(stderr, PUSH, this_Stack[STACK_COUNT_INDEX], item); 
            /*use hexpush if item in stack is negative*/
	    else
            fprintf(stderr, HEXPUSH, this_Stack[STACK_COUNT_INDEX], item); 
    }
    return 1;
        
}
/*---------------------------------------------------------------------------
Function Name:               top
Purpose:                     sends back an item through the output parameter
Description:                 sends back an item through the output parameter,
                             Returns true upon success and false upon failure
Input:                       pointer to the stack and the item to be toped
Output:                      top successful or not
Result:                      item sent, seccess or failure status returned
Side Effects:                None.
---------------------------------------------------------------------------*/
long top (Stack * this_Stack, long * item) {
    /*check for pointer*/
    if(this_Stack == NULL){
	    fprintf(stderr, TOP_NONEXIST);
	    return 0;
    }
    
    /*check item pointer*/
    if(item == NULL){
	    fprintf(stderr, INCOMING_NONEXIST);
	    return 0;
    }
    
    /*check for empty*/
    if(isempty_Stack(this_Stack)){
	    fprintf(stderr, TOP_EMPTY);
            return 0;
    }
   
    /*top the item*/
    *item = this_Stack[this_Stack[STACK_POINTER_INDEX]];
   
    /*debug messege*/
    if(debug){
	    if(this_Stack[this_Stack[STACK_POINTER_INDEX]]> 0) 
            fprintf(stderr, TOP, this_Stack[STACK_COUNT_INDEX], *item); 
            /*use hextop if item in stack is negative*/
	    else
            fprintf(stderr, HEXTOP, this_Stack[STACK_COUNT_INDEX], *item); 
    }
    return 1;
	
 }

FILE * write_Stack (Stack * this_Stack, FILE * stream) {

        long index = 0;         /* index into the stack */

        if (this_Stack == NULL) {
                fprintf (stderr, WRITE_NONEXIST_STACK);
                return stream;
        }

        if (stream == NULL) {
                fprintf (stderr, WRITE_NONEXIST_FILE);
                return stream;
        }
                
        if (stream == stderr)
                fprintf (stream, "Stack has %ld items in it.\n",
                        get_occupancy (this_Stack));

        for (index = STACK_COUNT_INDEX + STACK_OFFSET;
                index < get_occupancy (this_Stack); index++) {

                if (stream == stderr)
                        fprintf (stream, "Value on stack is |0x%lx|\n",
                                this_Stack[index]);
                else {
                        if (this_Stack[index] < 0)
                                fprintf (stream, "%c ",
                                        (char) this_Stack[index]);
                        else
                                fprintf (stream, "%ld ", this_Stack[index]);
                }
        }

        return stream;
}
