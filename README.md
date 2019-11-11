[//]: <>    (This markdown document was previewed in JetBrains' MD
            Previewer. I hope it renders well for you, as well.)

# Programming Assignment 2
### How to run...
  1) Create/modify `test.legv8asm` to include your own assembly code.
  2) Run `./legv8asm test.legv8asm -a` in your terminal to assemble that program into binary. 
     You will find `test.legv8asm.machine` has been generated as a binary file.
  3) Run `make` in your terminal to compile `assignment2.c`.
  4) Run `./assignment2 test.legv8asm.machine` to run `test.legv8asm.machine` 
     as binary assembly code and analyze the output.
     
  - Optional Steps Post Testing:
    1) Run `make clean` to clean up the generated files from the making of `assignment2.c`.
    2) Delete the generated `.machine` file to keep things clean.
    