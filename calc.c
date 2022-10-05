/****************************************************************************
File Name:      calc.c
Description:    This file contains the functions needed to perform positive 
                number calculations and solve expressions of calculations.

****************************************************************************/
#include <ctype.h>
#include <stdio.h>
#include "calc.h"
#include "mylib.h"
#include "stack.h"

#define CHARACTER(word) ((char) ((word) & 0x000000FF))
#define INDEX(word) (((word) & 0x0000FF00) >> 8)
#define PRIORITY(word) ((word) & 0x0000FE00)
#define SIGN_BIT (1L << ((sizeof (long) << 3) - 1))
#define ASCII_ZERO (48)  /*zero's ascii code*/
#define BYTE (8)   /*the size of a byte*/
#define IGNORED_NUM (1)  /*for ignored operand used in factorial operations*/

static char operators[] = "()+-*/^ !";


static long add (long, long);
static long divide (long, long);
static long exponent (long, long);
static long fact (long, long);
static long mult (long, long);
static long setupword (int);
static long sub (long, long);

static long (*functions[]) (long, long) =
    { NULL, NULL, add, sub, mult, divide, exponent, NULL, fact };

/*---------------------------------------------------------------------------
Function Name:                eval
Purpose:                      This function is used to calculate the result
Description:                  Use two stacks to calculate the result.
                              pop numbers from stack2, pushing all digits 
                              popped to stack1.  Once a non-numbers is
                              encountered from stack2, the appropriate operand
                              number will be popped from stack1 and evaluated
                              using the operator just popped from stack2.
                              The result of this computation will be
                              pushed on stack1.
Input:                        This function will take a pointer to a stack
                              as a parameter
Output:                       The calcultion result.
Result:                       Expression calculated, result returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/
long eval (Stack * stack1) {
        
	long tempChar;         /*used when popping and pushing between stacks*/
	long op1, op2, word;   /*for the operands and the operator*/
	long result;           /*used to store the result*/
	Stack * stack2 = new_Stack(CALCSTACKSIZE); /*create new stack*/
        
	while(!isempty_Stack(stack1)){     /*reverse stack1 onto stack2*/
		pop(stack1, &tempChar);
		push(stack2, tempChar);
	}
        while(!isempty_Stack(stack2)){
	        pop(stack2, &tempChar);	
		/*push the numbers to stack1*/
                if(tempChar >= 0){     
			push(stack1, tempChar);
		}	
	  /*calculate the result when hit operands and store it in stack1*/
	        else{
		        word = tempChar;
		       	pop(stack1, &op1);
			
			/*for factorial operator*/
		        if(word == setupword('!')){
				op2 = IGNORED_NUM;
			}
			/*other opertors*/
			else{
		                pop(stack1, &op2);
			}
                        result = functions[INDEX(word)](op1, op2);
		       	push(stack1, result);
		}
	}
		empty_Stack(stack1);         /*empty the stack*/
		delete_Stack(&stack2);        /*free memory*/
		return result;
}
/*---------------------------------------------------------------------------
Function Name:                intopost
Purpose:                      This function evaluates mathematical expressions 
                              from "postfix" notation.
Description:                  Use two stacks to evaluate mathematical
                              expressions. Ignore blank space. Continue to read 
                              digits until it reads a non-digit, converting
                              this number to decimal by calling decin() and
                              store this decimal number on stack1.
                              Push character '(' to stack2. if character is ')' 
                              then repeatedly pop stack2, pushing all symbols
                              popped from stack2 onto stack1 until the first'('
                              encountered is popped from stack2.Discard '(' and
                              ')'.Then repeatedly push to stack1 what is popped
                              from stack2 until stack2 is empty or stack2's top
                              symbol has a lower priority than the character
                              entered.  Then push the character onto stack2.
                              After processing all characters, it pops anything 
                              remaining on stack2, pushing all symbols popped
                              from stack2 to stack1.
Input:                        This function will take a pointer to a stack
                              as a parameter
Output:                       '0' or '1' representing evaluate process sucess
                              or not
Result:                       Stack1 now contains the post-fix expression,
                              in reverse order, evaluation status returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/
long intopost (Stack * stack1) {
     long inChar;          /*store the character from fgetc*/
     long tempChar;        /*used when popping/pushing from stack2 to stack1*/
     Stack * stack2 = new_Stack(CALCSTACKSIZE); /*create stack2*/
     inChar = fgetc(stdin);
     while(inChar != '\n'){
     
     if(inChar == EOF)  {   /*check for EOF*/
	     delete_Stack(&stack2);
	     return EOF;
     }
    
     if(inChar == ' '){    /*ignore blank space*/
	     inChar = fgetc(stdin);  /*replace blank input with the next char*/
             continue;
     }	     
     
            /*check for digit*/
     else if(inChar >= ASCII_ZERO && inChar <= ASCII_ZERO + 9){
	     ungetc(inChar, stdin);
             push(stack1, decin());
     }
     
            /*check '('*/
     else if(inChar == '(')
	     push(stack2, setupword(inChar));
     
     else if(inChar == ')'){
	    /*check '('*/
	    top(stack2, &tempChar);
	    while(tempChar != setupword('(')){
		     pop(stack2, &tempChar);
		     push(stack1, tempChar);
		     top(stack2, &tempChar);
	     }   
	     pop(stack2, &tempChar); /*discard '('*/
     }
     else{
	    /*check if stack is empty*/
	    while(!isempty_Stack(stack2)){
            /*check priority*/
            top(stack2, &tempChar);
	    if(PRIORITY(tempChar) < PRIORITY(setupword(inChar)))
	    break;
	    pop(stack2,&tempChar);
	    push(stack1, tempChar);
     }
	    push(stack2, setupword(inChar));       
     }
	inChar = fgetc(stdin);
     }
     
     while(!isempty_Stack(stack2)){      /*pop and push the remaining symbol*/
	     pop(stack2, &tempChar);
             push(stack1, tempChar);
     }
     delete_Stack(&stack2);
     return 1;
}


static long add (long augend, long addend) {
    return augend + addend;
}

static long divide (long divisor, long dividend) {
    return dividend / divisor;
}
/*---------------------------------------------------------------------------
Function Name:                exponent
Purpose:                      This function is used to calculate exponential
                              operations
Description:                  calculate exponents by calling mult() in a for
                              loop
Input:                        Two longs for the base and the power
Output:                       The calcultion result.
Result:                       exponents calculated, result returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/
static long exponent (long power, long base) {
       long result;   /*used to stored the result*/
       long index;    /*the number of times the base has been multiplied*/
       result = base; 
       for(index=1; index < power; index++){ 
	      result =  mult(result, base); 
       }
       return result;
}
/*---------------------------------------------------------------------------
Function Name:                fact
Purpose:                      This function is used to calculate factorial
                              operations
Description:                  calculate factorials by calling mult() in a while
                              loop
Input:                        Two longs, one for the number needed to be 
                              factored, another parameter is ignored
Output:                       The calcultion result.
Result:                       factorials calculated, result returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/
static long fact (long xxx, long ignored) {
       long result;   /*used to store the result after factorial*/
       result = xxx;
       while(xxx > 1){         
              result = mult(result, xxx-1);
	      xxx -= 1;
       }
       return result;
}

static long mult (long factorx, long factory) {
    return factorx * factory;
}

/*---------------------------------------------------------------------------
Function Name:               setupword
Purpose:                     Constructor funtion for longs representing 
                             operators to be stored on the Stacks. 
Description:                 The representation for the operators will have 
                             everything associated with that operator: a 
                             distinction from numbers, the index in the
                             functions array corresponding to that operator,
                             and the priority of the operator and the ASCIIcode
Input:                       This function will take the ASCII value of the
                             operater passd in
Output:                      longs representing operators
Result:                      longs constructed and returned.
Side Effects:                None.
---------------------------------------------------------------------------*/
static long setupword (int character) {
     long index = 0;    /*look for operater*/
    
     while(operators[index]){
	     if(character == operators[index])
		     break;
     index++;
     }
    
     return SIGN_BIT|(index << BYTE)|character;
     
}

static long sub (long subtrahend, long minuend) {
    return minuend - subtrahend;
}

