/*----------------------------------------------------------------------------
-		                    COM S 321: Assignment 2	    	                 -
-	Name: Steven Marshall Sheets  && Noah Frederiksen						 -
- 	Section: 1          													 -
-	NetID: smsheets@iastate.edu	  && nfred99@iastate.edu				     -
-	Date: October 21st, 2019												 -
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Defines 									 -
-----------------------------------------------------------------------------*/
#define MAXCHAR 1000

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdio.h>

/*----------------------------------------------------------------------------
-	                     Registers (Global Variables)                        -
-----------------------------------------------------------------------------*/
/* Preserved */
int X19, X20, X21, X22, X23, X24, X25, X26, X27;   /* Saved Registers                      */
int SP;                                            /* Stack Pointer Register (X28)         */
int FP;                                            /* Frame Pointer Register (X29)         */
int LR;                                            /* Link Register (X30) (return address) */
/* Stack above the stack pointer */

/* Not Preserved */
int X9, X10, X11, X12, X13, X14, X15;        /* Temporary Registers       */
int X0, X1, X2, X3, X4, X5, X6, X7;          /* Argument/Result Registers */
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
    // Step 1: Read in the file
    if (argc != 2)
    {
        printf("Program %s has been provided.\n"
               "However, no input file name was provided.\n"
               "Please run again with an input file.", argv[0]);
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Could not open file %s.", argv[1]);
        return 1;
    }

    char input[MAXCHAR];

    // Testing.
    printf("%s", input);
    while (fgets(input, MAXCHAR, fp)) {
        printf("%s", input);
    }
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    printf("\n%d", size);

    fclose(fp);



    // Divide the file by 32 bits. Each instruction is 32 bits long.
    // They are not new line delimited, according to Sheaffer.

    return 0;
}
