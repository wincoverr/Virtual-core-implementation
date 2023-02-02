#include <stdio.h>
#include <stdlib.h>



#include <stdint.h>
#include <string.h>
#include <math.h>



//gcc -m64 -Wall -g main.c -o main
void execute(){


}



void fetch(){

}



void decode(){


}

char *readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    char *code;
    size_t n = 0;
    int c;

    if (file == NULL) return NULL; //could not open file
    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    code = malloc(f_size);

    while ((c = fgetc(file)) != EOF) {
        code[n++] = (char)c;
    }

    code[n] = '\0';        

    return code;
}



int decimal_binary(int n)  /* Function to convert decimal to binary.*/
{
    int rem, i=1, binary=0;
    while (n!=0)
    {
        rem=n%2;
        n/=2;
        binary+=rem*i;
        i*=10;
    }
    return binary;
}


// et logique avec une valeur choisi => récupérer les bits qui m'interesent
// masque pipe 
int main(int argc, char *argv[]) {
    int tempBinary;
    char *returned_str = readFile("file.txt");
    for(int i=0;i<4;i++){
    tempBinary = decimal_binary((int)returned_str[i]);
    //tempBinary = to8bit(tempBinary);

    printf("Value: %d\n", tempBinary);    
    }

    //decimal_binary(100);
    decode();
    return 0;
}



