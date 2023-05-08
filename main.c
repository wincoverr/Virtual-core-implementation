#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include <arpa/inet.h> // htonl()

// gcc -m64 -g main.c -o main
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

uint32_t reg[16];


/*
Décode l'instruction à partir d'une valeur de 32 bits.Les bits sont extraits en suivant l'énoncé et stockés dans
une structure.
*/
Instruction decode(uint32_t *buff)
{
    Instruction instruction = {0};
    uint32_t mask = 0b00000000000000000000000011111111;

    uint32_t tmp = ntohl(*buff);

    instruction.IV = tmp & mask;
    mask = 0b00000000000000000000111100000000;
    instruction.dest = (tmp & mask) >> 8;
    mask = 0b00000000000000001111000000000000;
    instruction.op2 = (tmp & mask) >> 12;
    mask = 0b00000000000011110000000000000000;
    instruction.op1 = (tmp & mask) >> 16;
    mask = 0b00000000001111000000000000000000;
    instruction.opcode = (tmp & mask) >> 20;
    mask = 0b00000001000000000000000000000000;
    instruction.flag = (tmp & mask) >> 24;
    mask = 0b11110000000000000000000000000000;
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



/*

 je n'ai pas ajouté le code à mon programme, donc ça ne va pas marcher!
 Calcule la nouvelle valeur du pc
 Gère les différents types de conditions de branchement
*/
int32_t fetch(uint32_t pc, uint32_t *code, Instruction instruction) {
    uint32_t new_pc;
    bool take_branch = false;

    new_pc = pc;

    if (instruction.BBC == 0x8) {
        return new_pc;
    }
    if (instruction.BBC == 0x9) {
        if (reg[instruction.dest] == reg[instruction.op2]) {
            take_branch = true;
        }
    } else if (instruction.BBC == 0xA) {
        if (reg[instruction.dest] != reg[instruction.op2]) {
            take_branch = true;
        }
    } else if (instruction.BBC == 0xB) {
        if (reg[instruction.dest] <= reg[instruction.op2]) {
            take_branch = true;
        }
    } else if (instruction.BBC == 0xC) {
        if (reg[instruction.dest] >= reg[instruction.op2]) {
            take_branch = true;
        }
    } else if (instruction.BBC == 0xD) {
        if (reg[instruction.dest] < reg[instruction.op2]) {
            take_branch = true;
        }
    } else if (instruction.BBC == 0xE) {
        if (reg[instruction.dest] > reg[instruction.op2]) {
            take_branch = true;
        }
    }

    if (take_branch) {
        return new_pc;
    } else {
        return pc + 4;
    }
}



/*
    long case pour faire toutes les possibilités
*/
void execute(Instruction instruction)
{
    uint32_t op1 = reg[instruction.op1];
    uint32_t op2;
if (instruction.IV & 0x01000000) {
  op2 = instruction.IV & 0x00FFFFFF;
} else {
  op2 = reg[instruction.op2];
}
    switch (instruction.opcode)
    {
        case 0x0:  // AND
            reg[instruction.dest] = op1 & op2;
            break;

        case 0x1:  // OR
            reg[instruction.dest] = op1 | op2;
            break;

        case 0x2:  // XOR
            reg[instruction.dest] = op1 ^ op2;
            break;

        case 0x3:  // ADD
            reg[instruction.dest] = op1 + op2;
            break;

        case 0x4:  // ADC
            reg[instruction.dest] = op1 + op2 + reg[15];
            break;

        case 0x5:  // CMP
            if (op1 == op2)
            {
                reg[15] = 1 << 0;  // BEQ
            }
            else if (op1 < op2)
            {
                reg[15] = 1 << 2;  // BLE
            }
            else
            {
                reg[15] = 1 << 5;  // BG
            }
            break;

        case 0x6:  // SUB
            reg[instruction.dest] = op1 - op2;
            break;

        case 0x7:  // SBC
            reg[instruction.dest] = op1 - op2 - !reg[15];
            break;

        case 0x8:  // MOV
            reg[instruction.dest] = op2;
            break;

        case 0x9:  // LSH
            reg[instruction.dest] = op1 << op2;
            break;

        case 0xA:  // RSH
            reg[instruction.dest] = op1 >> op2;
            break;

        default:
            printf("ERROR: invalid opcode 0x%x\n", instruction.opcode);
            exit(1);
            break;
    }
}


int main(int argc, char *argv[])
{
    /*
    // Verification
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "BIN_NAME <CODE> <STATE> (VERBOSE)");
        return 1;
    }
    bool verbose = (argc == 4 && strcmp(argv[3], "VERBOSE") == 0);

    // Ouvre le fichier de code spécifié dans le premier argument, vérifie que le fichier est ouvert avec succès et calcule sa taille.
    // charge ensuite le contenu du fichier dans un tableau d'octets.
    FILE *codeF = fopen(argv[1], "rb");
    if (!codeF)
    {
        fprintf(stderr, "erreur sur le fichier code");
        return 1;
    }
    fseek(codeF, 0, SEEK_END);
    long code_size = ftell(codeF);
    rewind(codeF);
    uint8_t *code = (uint8_t *)malloc(code_size);
    fread(code, 1, code_size, codeF);
    fclose(codeF);

    // Ouvre le fichier d'état initial spécifié dans le deuxième argument, vérifie que le fichier est ouvert avec succès.
    FILE *stateF = fopen(argv[2], "r");
    if (!stateF)
    {
        fprintf(stderr, "erreur sur le fichier state");
        return 1;
    }

    // Je ne sais pas trop quoi faire après avec ce fichier
*/
  
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