#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <string.h>
#include <math.h>

// gcc -m64 -Wall -g main.c -o main
typedef struct instruction
{
    uint32_t IV;
    uint32_t dest;
    uint32_t op2;
    uint32_t op1;
    uint32_t opcode;
    uint32_t flag;
    uint32_t BBC;
} Instruction;

typedef struct core
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t r13;
    uint32_t r14;
} Core;




Instruction decode(uint32_t *buff)
{
    Instruction instruction = {0};
    uint32_t mask = 0xff;

    instruction.IV = *buff & mask;
    mask = 0xf00;
    instruction.dest = (*buff & mask) >> 8;
    mask = 0xf000;
    instruction.op2 = (*buff & mask) >> 12;
    mask = 0xf0000;
    instruction.op1 = (*buff & mask) >> 16;
    mask = 0xf00000;
    instruction.opcode = (*buff & mask) >> 20;
    mask = 0x1000000;
    instruction.flag = (*buff & mask) >> 24;
    mask = 0xf0000000;
    instruction.BBC = (*buff & mask) >> 28;
    return instruction;
}
/*
Instruction decode(uint32_t *buff)
{
    Instruction instruction = {0};
    uint32_t comparaison = 255;

    instruction.IV = *buff & comparaison;
    comparaison = 3840;
    instruction.dest = (*buff & comparaison) >> 8;
    comparaison = 61440;
    instruction.op2 = (*buff & comparaison) >> 12;
    comparaison = 983040;
    instruction.op1 = (*buff & comparaison) >> 16;
    comparaison = 15728640;
    instruction.opcode = (*buff & comparaison) >> 20;
    comparaison = 16777216;
    instruction.flag = (*buff & comparaison) >> 24;
    comparaison = 4026531840;
    instruction.BBC = (*buff & comparaison) >> 28;

    /*


    Value: 1100001  97     61   comparaison = 255;
    Value: 1100010  98     62   65280
    Value: 1100011  99     63   16711680
    Value: 1100100  100    64   4278190080


        /* 01510020 
        64636261

        little endian

    iv : 20
    dest: 0
    op2 :0
    op1 : 1
    operation code: 5
    flag: 1
    bcc : 0

    
    return instruction;
}*/

char *readFile(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    char *code;
    size_t n = 0;
    uint64_t c;

    if (file == NULL)
        return NULL; // could not open file
    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    code = malloc(f_size);

    while ((c = fgetc(file)) != EOF)
    {
        code[n++] = (char)c;
    }

    code[n] = '\0';

    return code;
}

int main(int argc, char *argv[])
{
    /**/
    uint8_t *returned_str = readFile("file.bin");
    uint32_t *buff = (uint32_t *)returned_str;
    printf("%lx\n", buff[0]);
    Instruction instruction = decode(buff);

    printf("IV: %lx\n", instruction.IV);
    printf("dest: %lx\n", instruction.dest);
    printf("op2: %lx\n", instruction.op2);
    printf("op1: %lx\n", instruction.op1);
    printf("opcode: %lx\n", instruction.opcode);
    printf("flag: %lx\n", instruction.flag);
    printf("BBC: %lx\n", instruction.BBC);

    uint32_t PC = 0;
    int32_t offset = (int32_t)instruction.IV;
    switch (10)
    {
    case 0x8: // Unconditional branch
        PC = (uint32_t)offset;
        break;
    case 0x9: // Branch if equal
        if (instruction.flag)
        {
            PC = (uint32_t)(offset + 4); // add 4 to skip the next instruction
        }
        else
        {
            PC = (uint32_t)(PC + 4); // execute the next instruction
        }
        break;
    case 0xa: // Branch if not equal
        if (!instruction.flag)
        {
            PC = (uint32_t)(offset + 4);
        }
        else
        {
            PC = (uint32_t)(PC + 4);
        }
        break;
    case 0xb: // Branch if lower or equal
        if (instruction.flag || instruction.op1 == instruction.op2)
        {
            PC = (uint32_t)(offset + 4);
        }
        else
        {
            PC = (uint32_t)(PC + 4);
        }
        break;
    case 0xc: // Branch if greater or equal
        if (instruction.flag || instruction.op1 > instruction.op2)
        {
            PC = (uint32_t)(offset + 4);
        }
        else
        {
            PC = (uint32_t)(PC + 4);
        }
        break;
    case 0xd: // Branch if lower
        if (!instruction.flag && instruction.op1 < instruction.op2)
        {
            PC = (uint32_t)(offset + 4);
        }
        else
        {
            PC = (uint32_t)(PC + 4);
        }
        break;
    case 0xe: // Branch if gr   eater
        if (!instruction.flag && instruction.op1 > instruction.op2)
        {
            PC = (uint32_t)(offset + 4);
        }
        else
        {
            PC = (uint32_t)(PC + 4);
        }
        break;
    default: // any other value
        PC = (uint32_t)(PC + 4);
        break;
    }

    // Print the result
    printf("PC: %u\n", PC);
    return 0;
}