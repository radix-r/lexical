/*

COP 3402
HW2 - Scanner

Author: Ross Wagner

This program reads in the name of a PL0 file from the command line and outputs the source
program, a Lexeme Table, and a Lexeme List to stdout if there are no lexical errors
in the source program. If there are lexical errors an error message is printed
giving a brief discription of the nature of the error

compile using gcc lexer.c
*/



#include <stdio.h>
#include <stdlib.h>


typedef enum {
nulsym = 1, identsym, numbersym, plussym, minussym,
multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym,
whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
readsym , elsesym } token_type;


// signitures
char * fileToStr(char * fName);

int main(int argc, char** argv){

  if(argc != 2){
    printf("Usage: ./lex <PM0 source file>\n" );
    return -1;
  }

  // read file into a string
  char* buffer = fileToStr(argv[1]);

  fprintf(stdout, "Source Program:%s\n", argv[1]);
  printf("%s\n", buffer);








}

/**
method to read whole input file into buffer

source: https://stackoverflow.com/questions/174531/easiest-way-to-get-files-contents-in-c#174552

doesnt check error-codes for fseek, ftell and fread

bad for files > 4GB
*/
char * fileToStr(char * fName){

  char * buff = 0;
  long length;

  FILE* f = fopen(fName, "rb");

  if(f){
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buff = malloc (length+1);
    if (buff){
      fread (buff, 1, length, f);
    }
    fclose (f);
    buff[length] = '\0';
  }
  else{
    printf("Failed to open %s.\n", fName);
    return NULL;
  }

  return buff;

}
