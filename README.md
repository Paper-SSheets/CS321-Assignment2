[//]: <>    (This markdown document was previewed in JetBrains' MD
            Previewer. I hope it renders well for you, as well.)

# Programming Assignment 2

## File Descriptions
  1) `legv8asm` is an [LEGv8](https://www.google.com/search?rlz=1C1GCEB_enUS874US874&sxsrf=ACYBGNSyPcRY0mOYag4LpZRK1g_NU0OFVQ%3A1573434670376&ei=LrXIXf23FtbV-gSJ-IPgCg&q=LEGv8&oq=LEGv8&gs_l=psy-ab.3..35i39j0i20i263j0l8.4089.7800..8193...3.1..0.134.880.2j6......0....1..gws-wiz.......0i71j0i67j0i131j0i30j0i10i30j0i8i30.dhnjDA-Rt6M&ved=0ahUKEwj9huv0_ODlAhXWqp4KHQn8AKwQ4dUDCAs&uact=5) 
     instruction set emulator that is used to produce a binary output of an assembled program. Created by the instructor, Jeremy Sheaffer.
  2) `test.legv8asm` is a LEGv8 assembly program as a plain text file.
  3) `test.legv8asm.machine` is the binary version of `test.legv8asm` after being assembled 
     using `./legv8asm test.legv8asm -a`.
     
## How to run...
  1) Create/modify `test.legv8asm` to include your own assembly code.
  2) Run `./legv8asm test.legv8asm -a` in your terminal to assemble that program into binary. 
     You will find `test.legv8asm.machine` has been generated as a binary file.
  3) Run `make` in your terminal to compile `assignment2.c`.
  4) Run `./assignment2 test.legv8asm.machine` to run `test.legv8asm.machine` 
     as binary assembly code and analyze the output.
     
  - Optional Steps Post Testing:
    1) Run `make clean` to clean up the generated files from the making of `assignment2.c`.
    2) Delete the generated `.machine` file to keep things clean.
    