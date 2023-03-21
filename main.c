#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <arpa/inet.h>  // htonl()

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

Instruction decode(uint32_t *buff)
{
    Instruction instruction = {0};
    uint32_t mask = 0xff;

    uint32_t tmp = ntohl(*buff); 

    instruction.IV = tmp & mask;
    mask = 0xf00;
    instruction.dest = (tmp & mask) >> 8;
    mask = 0xf000;
    instruction.op2 = (tmp & mask) >> 12;
    mask = 0xf0000;
    instruction.op1 = (tmp & mask) >> 16;
    mask = 0xf00000;
    instruction.opcode = (tmp & mask) >> 20;
    mask = 0x1000000;
    instruction.flag = (tmp & mask) >> 24;
    mask = 0xf0000000;
    instruction.BBC = (tmp & mask) >> 28;
    return instruction;
}


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

    return 0;
}