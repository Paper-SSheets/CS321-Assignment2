/*----------------------------------------------------------------------------
-		                    COM S 321: Assignment 2	    	                 -
-	Name: Steven Marshall Sheets  && Noah Frederiksen    && Tyler Norris	 -
- 	Section: 1          													 -
-	NetID: smsheets 	          && nfred99	         && tjnorris	     -
-	Date: October 21st, 2019												 -
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

/*----------------------------------------------------------------------------
-	                            Prototypes                                   -
-----------------------------------------------------------------------------*/
char *int32_to_bin(int32_t n, int useSpace);

int find_instruction(uint32_t instruction);

int32_t run_instruction(uint32_t instruction);

uint32_t *parse_registers(uint32_t instruction);

void execute_program(int32_t instructions[], uint32_t size);

void initialize_machine_state();

void print_dump();

void set_condition_registers(int32_t result);

/*----------------------------------------------------------------------------
-	                            Structs                                      -
-----------------------------------------------------------------------------*/
struct machine_state
{
    char *instruction_strings[37];

    char *instruction_types[6];

    // Current place in assembly.
    int32_t pc;

    int32_t registers[32];

    int32_t formats[37];

    // Find the condition register by >> the number of bits.
    uint8_t condition_registers[14];

    // Main stack: an array of 8-bit integers.
    uint8_t *stack;

    // Main memory: an array of 8-bit integers.
    uint8_t *memory;

    // In 0b<opcode> format.
    uint32_t opcodes[37]; // 37 is the number of unique instructions.

    // Instruction Formats
    const enum instruction_format
    {
        fR,
        fI,
        fD,
        fB,
        fCB,
        fIW
    } instruction_format;

    // Instruction Names
    const enum instruction_names
    {
        ADD,
        ADDI,
        AND,
        ANDI,
        B,
        Bcond,
        BL,
        BR,
        CBNZ,
        CBZ,
        DUMP,
        EOR,
        EORI,
        HALT,
        LDUR,
        LDURB,
        LDURH,
        LDURSW,
        LSL,
        LSR,
        MUL,
        ORR,
        ORRI,
        PRNL,
        PRNT,
        SDIV,
        SMULH,
        STUR,
        STURB,
        STURH,
        STURW,
        SUB,
        SUBI,
        SUBIS,
        SUBS,
        UDIV,
        UMULH
    } instruction_names;

    const enum conditions
    {
        cEQ,
        cNE,
        cHS,
        cLO,
        cMI,
        cPL,
        cVS,
        cVC,
        cHI,
        cLS,
        cGE,
        cLT,
        cGT,
        cLE
    } conditions;
};

/*----------------------------------------------------------------------------
-	                           Global Variables                              -
-----------------------------------------------------------------------------*/
struct machine_state machine_state;

/*----------------------------------------------------------------------------
-								Implementation								 -
-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    // Read in the file.
    if (argc != 2)
    {
        printf("Program %s has been provided.\n"
               "However, no input file name was provided.\n"
               "Please run again with an input file.", argv[0]);
    }

    // Open the file.
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        printf("Could not open file %s.\n"
               "Perhaps it is corrupted?", argv[1]);
        return 1;
    }

    // Go to the end of the file to find its size in bytes.
    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    uint32_t instructions[file_size / 4]; // Divide by 4 because file_size is in bytes.

    // Go back to the start of the file.
    rewind(fp);

    // Initialize the state of the machine.
    initialize_machine_state();

    // Read through the file.
    int i = 0;
    uint32_t instruction;
    while (fread(&instruction, sizeof(uint32_t), 1, fp))
    {
        instruction = be32toh(instruction); // Convert the instruction to host endianness.
        instructions[i] = instruction;
        i++;
    }

    // Print the statistics of the file.
    printf("File Size: %u Bytes\n", file_size);
    printf("Instruction Count: %u\n", file_size / 4);
    printf("\nInstructions: \n");

    // Print the instructions.
    for (int i = 0; i < sizeof(instructions) / sizeof(instructions[0]); i++)
    {
        int instruction_index = find_instruction(instructions[i]);
        printf("%d|", instruction_index);
        printf("%s\t", machine_state.instruction_strings[instruction_index]);
        printf("%d|", machine_state.formats[instruction_index]);
        printf("%s\t", machine_state.instruction_types[machine_state.formats[instruction_index]]);

        int *registers = parse_registers(instructions[i]);
        printf(" %d ", registers[0]);
        printf(" %d ", registers[1]);
        printf(" %d ", registers[2]);
        puts("");
    }

    execute_program(instructions, file_size / 4);
    return 0;
}


/*----------------------------------------------------------------------------
-	                            Functions                                    -
-----------------------------------------------------------------------------*/
void initialize_machine_state()
{
    uint32_t instruction_opcodes[] = {
            0b10001011000,  // ADD
            0b1001000100,   // ADDI
            0b10001010000,  // AND
            0b1001001000,   // ANDI
            0b000101,       // B
            0b01010100,     // Bcond
            0b100101,       // BL
            0b11010110000,  // BR
            0b10110101,     // CBNZ
            0b10110100,     // CBZ
            0b11111111110,  // DUMP
            0b11001010000,  // EOR
            0b1101001000,   // EORI
            0b11111111111,  // HALT
            0b11111000010,  // LDUR
            0b00111000010,  // LDURB
            0b01111000010,  // LDURH
            0b10111000100,  // LDURSW
            0b11010011011,  // LSL
            0b11010011010,  // LSR
            0b10011011000,  // MUL
            0b10101010000,  // ORR
            0b1011001000,   // ORRI
            0b11111111100,  // PRNL
            0b1111111101,   // PRNT
            0b10011010110,  // SDIV
            0b10011011010,  // SMULH
            0b11111000000,  // STUR
            0b00111000000,  // STURB
            0b01111000000,  // STURH
            0b10111000000,  // STURW
            0b11001011000,  // SUB
            0b1101000100,   // SUBI
            0b1111000100,   // SUBIS
            0b1110101000,   // SUBS
            0b10011010110,  // UDIV
            0b10011011110   // UMULH
    };

    int32_t instruction_formats[] = {
            fR, // ADD
            fI, // ADDI
            fR, // AND
            fI, // ANDI
            fB, // B
            fCB, // Bcond
            fB, // BL
            fR, // BR
            fCB, // CBNZ
            fCB, // CBZ
            fR, // DUMP
            fR, // EOR
            fI, // EORI
            fR, // HALT
            fD, // LDUR
            fD, // LDURB
            fD, // LDURH
            fD, // LDURSW
            fR, // LSL
            fR, // LSR
            fR, // MUL
            fR, // ORR
            fI, // ORRI
            fR, // PRNL
            fR, // PRNT
            fR, // SDIV
            fR, // SMULH
            fD, // STUR
            fD, // STURB
            fD, // STURH
            fD, // STURW
            fR, // SUB
            fI, // SUBI
            fI, // SUBIS
            fR, // SUBS
            fR, // UDIV
            fR, // UMULH
    };

    int32_t registers[] = {
            0, // X0 Return
            0, // X1 Arg
            0, // X2 Arg
            0, // X3 Arg
            0, // X4 Arg
            0, // X5 Arg
            0, // X6 Arg
            0, // X7 Arg
            0, // X8 Indirect Result
            0, // X9 Temp
            0, // X10 Temp
            0, // X11 Temp
            0, // X12 Temp
            0, // X13 Temp
            0, // X14 Temp
            0, // X15 Temp
            0, // X16 IP0
            0, // X17 IP1
            0, // X18 Temp
            0, // X19 Saved
            0, // X20 Saved
            0, // X21 Saved
            0, // X22 Saved
            0, // X23 Saved
            0, // X24 Saved
            0, // X25 Saved
            0, // X26 Saved
            0, // X27 Saved
            0, // X28 Stack Pointer (SP)
            0, // X29 Frame Pointer (FP)
            0, // X30 Link Register / Return Address (LR)
            0, // X31 XZR
    };

    char *instruction_strings[] = {
            "ADD",
            "ADDI",
            "AND",
            "ANDI",
            "B",
            "Bcond",
            "BL",
            "BR",
            "CBNZ",
            "CBZ",
            "DUMP",
            "EOR",
            "EORI",
            "HALT",
            "LDUR",
            "LDURB",
            "LDURH",
            "LDURSW",
            "LSL",
            "LSR",
            "MUL",
            "ORR",
            "ORRI",
            "PRNL",
            "PRNT",
            "SDIV",
            "SMULH",
            "STUR",
            "STURB",
            "STURH",
            "STURW",
            "SUB",
            "SUBI",
            "SUBIS",
            "SUBS",
            "UDIV",
            "UMULH"
    };

    char *instruction_types[] = {
            "R",
            "I",
            "D",
            "B",
            "CB",
            "IW"
    };

    memcpy(machine_state.opcodes, instruction_opcodes, sizeof(instruction_opcodes));
    memcpy(machine_state.formats, instruction_formats, sizeof(instruction_formats));
    memcpy(machine_state.instruction_strings, instruction_strings, sizeof(instruction_strings));
    memcpy(machine_state.instruction_types, instruction_types, sizeof(instruction_types));
    memcpy(machine_state.registers, registers, sizeof(registers));
}

void execute_program(int32_t instructions[], uint32_t size)
{
    while (machine_state.pc >= 0 && machine_state.pc < size)
    {
        int32_t dPC = run_instruction(instructions[machine_state.pc]);
        machine_state.pc += dPC;
    }
}


int32_t run_instruction(uint32_t instruction)
{
    int32_t *registers = parse_registers(instruction);
    int32_t instruction_index = find_instruction(instruction);

    switch (instruction_index)
    {
        case ADD: // rd = rn + rm
        {
            int32_t rd = registers[0];
            int32_t rn = registers[1];
            int32_t rm = registers[2];
            machine_state.registers[rd] = machine_state.registers[rn] + machine_state.registers[rm];
            return 1;
        }
        case ADDI: // rd = rn + #im
        {
            int32_t rd = registers[0];
            int32_t rn = registers[1];
            int32_t im = registers[2];
            machine_state.registers[rd] = machine_state.registers[rn] + im;
            return 1;
        }
        case AND:
            return 1;
        case ANDI:
        {
            return 1;
        }
        case B:
        {
            int32_t adr = registers[0];
            return adr;
        }
        case Bcond:
        {
            int32_t rd = registers[0];
            int32_t adr = registers[1];
            if (machine_state.condition_registers[rd])
            {
                return adr;
            }
            return 1;
        }
        case BL:
        {
            int32_t adr = registers[0];
            machine_state.registers[30] = machine_state.pc + 1; // X30 = LR
            return adr;
        }
        case BR:
        {
            int32_t rd = registers[0];
            return machine_state.registers[30] - machine_state.pc; // X30 = LR
        }
        case CBNZ:
        {
            int32_t rd = registers[0];
            int32_t adr = registers[1];
            return machine_state.registers[rd] != 0 ? adr : 1;
        }
        case CBZ:
        {
            int32_t rd = registers[0];
            int32_t adr = registers[1];
            return machine_state.registers[rd] == 0 ? adr : 1;
        }
        case DUMP: // Prints registers, memory, and stack.
        {
            print_dump();
            return 1;
        }
        case EOR:
            return 1;
        case EORI:
            return 1;
        case HALT:
        {
            print_dump();
            machine_state.pc = -1; // Terminate the program.
            return 0;
        }
        case LDUR:
            return 1;
        case LDURB:
            return 1;
        case LDURH:
            return 1;
        case LDURSW:
            return 1;
        case LSL:
        {
            uint32_t rd = registers[0];
            uint32_t rn = registers[1];
            uint32_t shamt = registers[3];
            machine_state.registers[rd] = machine_state.registers[rn] << shamt;
            return 1;
        }
        case LSR:
        {
            uint32_t rd = registers[0];
            uint32_t rn = registers[1];
            uint32_t shamt = registers[3];
            machine_state.registers[rd] = machine_state.registers[rn] >> shamt;
            return 1;
        }
        case MUL:
            return 1;
        case ORR:
            return 1;
        case ORRI:
            return 1;
        case PRNL:
        {
            puts("");
            return 1;
        }
        case PRNT: // Print out a register.
        {
            uint32_t rd = registers[0];
            printf("X%d\t%x\t%d", rd, machine_state.registers[rd], machine_state.registers[rd]);
            return 1;
        }
        case SDIV:
            return 1;
        case SMULH:
            return 1;
        case STUR:
            return 1;
        case STURB:
            return 1;
        case STURH:
            return 1;
        case STURW:
            return 1;
        case SUB:
        {
            int32_t rd = registers[0];
            int32_t rn = registers[1];
            int32_t rm = registers[2];
            int32_t result = machine_state.registers[rn] - machine_state.registers[rm];
            machine_state.registers[rd] = result;
            return 1;
        }
        case SUBI:
        {
            int32_t rd = registers[0];
            int32_t rn = registers[1];
            int32_t im = registers[2];
            machine_state.registers[rd] = machine_state.registers[rn] - im;
            return 1;
        }
        case SUBIS:
        {
            int32_t rd = registers[0];
            int32_t rn = registers[1];
            int32_t im = registers[2];
            int32_t result = machine_state.registers[rn] - im;
            machine_state.registers[rd] = result;
            set_condition_registers(result);
            return 1;
        }
        case SUBS:
        {
            int32_t rd = registers[0];
            int32_t rn = registers[1];
            int32_t rm = registers[2];
            int32_t result = machine_state.registers[rn] - machine_state.registers[rm];
            machine_state.registers[rd] = result;
            set_condition_registers(result);
            return 1;
        }
        case UDIV:
            return 1;
        case UMULH:
            return 1;
        default: // Terminate the program like in HALT.
        {
            machine_state.pc = -1;
            return 0;
        }
    }
}

/**
 * Prints registers, memory, and stack, i.e., the dump.
 */
void print_dump()
{
    printf("\n--------------------------------------------------------------------------\n");
    printf("-                                 Registers                              -\n");
    printf("--------------------------------------------------------------------------\n");
    printf("Reg\tHex\t\tBinary\t\t\t\t\t\tBase 10\n");

    for (int i = 0; i < sizeof(machine_state.registers) / sizeof(machine_state.registers[0]); i++)
    {
        printf("X%d\t", i);
        printf("%x\t\t", machine_state.registers[i]);
        printf("%s\t\t", int32_to_bin(machine_state.registers[i], 0));
        printf("%d\t", machine_state.registers[i]);
        puts("");
    }
}

/**
 * Sets the condition registers based on a given subtraction result.
 *
 * @param result - The subtraction result used to modify the condition registers.
 */
void set_condition_registers(int32_t result)
{
    // Reset all condition registers to their default state.
    for (int i = cEQ; i <= cVS; i++)
    {
        machine_state.condition_registers[i] = 0;
    }

    if (result == 0)
    {
        machine_state.condition_registers[cEQ] = 1;
    }
    if (result >= 0)
    {
        machine_state.condition_registers[cGE] = 1;
        machine_state.condition_registers[cHS] = 1;
        machine_state.condition_registers[cPL] = 1;
        machine_state.condition_registers[cNE] = 1;
    }
    if (result > 0)
    {
        machine_state.condition_registers[cGT] = 1;
        machine_state.condition_registers[cHI] = 1;
        machine_state.condition_registers[cNE] = 1;
    }
    if (result <= 0)
    {
        machine_state.condition_registers[cLE] = 1;
        machine_state.condition_registers[cLS] = 1;
        machine_state.condition_registers[cNE] = 1;
    }
    if (result < 0)
    {
        machine_state.condition_registers[cLO] = 1;
        machine_state.condition_registers[cLT] = 1;
        machine_state.condition_registers[cMI] = 1;
        machine_state.condition_registers[cNE] = 1;
    }
}

/**
 * Converts an int32_t n to binary.
 * @param number - The int32_t to convert.
 * @param space - Space for conversion.
 * @return - The converted int32_t in binary.
 */
char *int32_to_bin(int32_t number, int space)
{
    char *binary = malloc(sizeof(char) * (32 + 4));
    for (int i = 31; i >= 0; i--)
    {
        int j = number >> i;
        if (j & 1)
        {
            strcat(binary, "1");
        } else
        {
            strcat(binary, "0");
        }
        if (space && i % 8 == 0)
        {
            strcat(binary, " ");
        }
    }
    return binary;
}

/**
 * Finds the matching instruction in the machine_state's list of
 * opcodes and returns that index.
 *
 * @param instruction - The instruction to match with.
 * @return - The index of machine_state's list of opcodes that matches the instruction.
 */
int find_instruction(uint32_t instruction)
{
    for (int i = 0; i < 37; i++)
    {
        if (machine_state.opcodes[i] == ((instruction >> (32 - 6)) & 0b111111))
        {   // B instructions
            return i;
        } else if (machine_state.opcodes[i] == ((instruction >> (32 - 8)) & 0b11111111))
        {   // CB instructions
            return i;
        } else if (machine_state.opcodes[i] == ((instruction >> (32 - 10)) & 0b1111111111))
        {   // I instructions
            return i;
        } else if (machine_state.opcodes[i] == ((instruction >> (32 - 11)) & 0b11111111111))
        {   // R instructions
            return i;
        } else if (machine_state.opcodes[i] == ((instruction >> (32 - 11)) & 0b11111111111))
        {   // D instructions
            return i;
        }
    }
    return -1;
}

/**
 * Parse register values from an instruction binary.
 *
 * @param instruction - An instruction binary.
 * @return - An integer array with [0]:rd -- [1]:rm -- [2]:rm/im
 */
uint32_t *parse_registers(uint32_t instruction)
{
    // Get format of given instruction
    int instruction_index = find_instruction(instruction);
    int32_t instruction_opcode = machine_state.opcodes[instruction_index];
    int instruction_type = machine_state.formats[instruction_index];
    int32_t *registers = malloc(4 * sizeof(int32_t)); // registers[0] = rd, registers[1] = rn, registers[2] = rm/im
    registers[0] = -1;
    registers[1] = -1;
    registers[2] = -1;

    int32_t rd, rn, rm, im, adr, shamt;

    // TODO change binary to hex
    switch (instruction_type)
    {
        case fR:    // 11 opcode -- 5 rm -- 6 shamt -- 5 rn -- 5 rd
        {
            rd = instruction & 0b11111;
            rn = (instruction >> 5) & 0b11111;
            shamt = (instruction >> (5 + 5)) & 0b111111;
            rm = (instruction >> (5 + 5 + 6)) & 0b11111;
            registers[0] = rd;
            registers[1] = rn;
            registers[2] = rm;
            registers[3] = shamt;
            break;
        }
        case fI:    // 10 opcode -- 12 immediate -- 5 rn -- 5 rd
        {
            rd = instruction & 0b11111;
            rn = (instruction >> 5) & 0b11111;
            im = (instruction >> (5 + 5)) & 0b111111111111;
            registers[0] = rd;
            registers[1] = rn;
            registers[2] = im;
            break;
        }
        case fD:    // 11 opcode -- 9 offset -- 2 op -- 5 rn -- 5 rt
        {
            rd = instruction & 0b11111;
            rn = (instruction >> 5) & 0b11111;
            adr = (instruction >> (5 + 5 + 2)) & 0b111111111;
            registers[0] = rd;
            registers[1] = rn;
            registers[2] = adr;
            break;
        }
        case fB:    // 6 opcode -- 26 address
        {
            adr = instruction & 0b11111111111111111111111111;
            // Negative check.
            if ((instruction >> 5) & 0b10000000000000000000000000)
            {
                adr = -((~instruction >> 5) & 0b01111111111111111111111111) - 1;
            }
            registers[0] = adr;
            break;
        }
        case fCB:    // 8 opcode -- 19 address -- 5 rt
        {
            rd = instruction & 0b11111;
            adr = (instruction >> 5) & 0b1111111111111111111;
            // Negative check.
            if ((instruction >> 5) & 0b1000000000000000000)
            {
                adr = -((~instruction >> 5) & 0b0111111111111111111) - 1;
            }
            registers[0] = rd;
            registers[1] = adr;
            break;
        }
        case fIW:    // 11 opcode -- 16 immediate -- 5 rd
        {
            rd = instruction & 0b11111;
            im = (instruction >> 5) & 0b1111111111111111;
            break;
        }
        default:
            break;
    }
    return registers;
}
