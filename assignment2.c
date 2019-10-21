/*----------------------------------------------------------------------------
-		                    COM S 321: Assignment 2	    	                 -
-	Name: Steven Marshall Sheets  && Noah Frederiksen						 -
- 	Section: 1          													 -
-	NetID: smsheets@iastate.edu	  && nfred99@iastate.edu				     -
-	Date: October 21st, 2019												 -
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdio.h>

/*----------------------------------------------------------------------------
-	                     Registers (Global Variables)                        -
-----------------------------------------------------------------------------*/
/* Preserved */
int X19, X20, X21, X22, X23, X24, X25, X26, X27   /* Saved Registers                */
int X28 // OR SP, Noah, let's decide              /* Stack Pointer Register         */
int X29 // Or FP, Noah, let's decide              /* Frame Pointer Register         */
int X30 // Or LR, Noah, let's decide              /* Link Register (return address) */
/* Stack above the stack pointer */

/* Not Preserved */
int X9, X10, X11, X12, X13, X14, X15        /* Temporary Registers       */
int X0, X1, X2, X3, X4, X5, X6, X7          /* Argument/Result Registers */
/* Stack below the stack pointer */


/*----------------------------------------------------------------------------
-	                            Prototypes                                   -
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-	                                Notes                                    -
-----------------------------------------------------------------------------*/
// Compile with gcc lab03-1.c -o lab03-1
// Run with ./ds4rd.exe -d 054c:05c4 -D DS4_BT -t -a | ./lab03-1

/*----------------------------------------------------------------------------
-								Implementation								 -
-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    return 0;
}
