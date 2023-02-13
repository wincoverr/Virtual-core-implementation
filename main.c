#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <string.h>
#include <math.h>

// gcc -m64 -Wall -g main.c -o main
typedef struct instruction {
  uint32_t IV;
  uint32_t dest;
  uint32_t op2;
  uint32_t op1;
  uint32_t opcode;
  uint32_t flag;
  uint32_t BBC;
} Instruction;

Instruction getInstructionData(uint32_t *buff) {
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
    uint8_t *returned_str = readFile("fileASCII.txt");
    uint32_t *buff = (uint32_t *)returned_str;
    printf("%lx\n", buff[0]);
    Instruction instruction = getInstructionData(buff);
    
    printf("IV: %lx\n", instruction.IV);
    printf("dest: %lx\n", instruction.dest);
    printf("op2: %lx\n", instruction.op2);
    printf("op1: %lx\n", instruction.op1);
    printf("opcode: %lx\n", instruction.opcode);
    printf("flag: %lx\n", instruction.flag);
    printf("BBC: %lx\n", instruction.BBC);
    
    return 0;
}