#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <string.h>
#include <math.h>

// gcc -m64 -Wall -g main.c -o main
void execute()
{
}

void fetch()
{
}

void decode()
{
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

int decimal_binary(int n) /* Function to convert decimal to binary.*/
{
    uint64_t rem, i = 1, binary = 0;
    while (n != 0)
    {
        rem = n % 2;
        n /= 2;
        binary += rem * i;
        i *= 10;
    }
    return binary;
}


// et logique avec une valeur choisi => récupérer les bits qui m'interesent
// masque pipe
int main(int argc, char *argv[])
{
    // on peut crer les registredans un tableau
    uint8_t *returned_str = readFile("fileASCII.txt");

    uint32_t *buff = (uint32_t *)returned_str;
    //printf("%lx\n", buff[0]);
    
    
    //get IV:
    uint32_t comparaison = 255;
    uint32_t instructionIV = *buff & comparaison;
    printf("%lx\n", instructionIV);
 
    //get dest

    comparaison = 3840;
    uint32_t instructionDesk = *buff & comparaison;
    instructionDesk = instructionDesk >> 8;
    printf("%lx\n", instructionDesk);

    //get op2 

    comparaison = 61440;
    uint32_t instructionOp2 = *buff & comparaison;
    instructionOp2 = instructionOp2 >> 12;
    printf("%lx\n", instructionOp2);

    //get op1

    comparaison = 983040;
    uint32_t instructionOp1 = *buff & comparaison;
    instructionOp1 = instructionOp1 >> 16;
    printf("%lx\n", instructionOp1);


  //get opcode

    comparaison = 15728640;
    uint32_t instructionOpcode = *buff & comparaison;
    instructionOpcode = instructionOpcode >> 20;
    printf("%lx\n", instructionOpcode);

    //get flag

    comparaison = 16777216;
    uint32_t instructionflag = *buff & comparaison;
    instructionflag = instructionflag >> 24;
    printf("%lx\n", instructionflag);

    //get BBC

    comparaison = 4026531840;
    uint32_t instructionBBC = *buff & comparaison;
    instructionBBC = instructionBBC >> 28;
    printf("%lx\n", instructionBBC);

    
    /*

    Value: 1100001  97     61   comparaison = 255;
    Value: 1100010  98     62   65280
    Value: 1100011  99     63   16711680
    Value: 1100100  100    64   4278190080

    */

    return 0;
}
