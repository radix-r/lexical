/*

COP 3402
HW2 - Scanner

Author: Ross Wagner

This program reads in the name of a PL0 file from the command line and outputs the source
program, a Lexeme Table, and a Lexeme List to stdout if there are no lexical errors
in the source program. If there are lexical errors an error message is printed
giving a brief discription of the nature of the error

compile using gcc lexer.c


To-Do
-----


*/


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_LEN 20
#define MAX_NUM_LEN 5
#define MAX_IDENTIFIER_LEN 11

#define OK 0
#define ERROR -1
#define END 1

typedef enum {
nulsym = 1, identsym, numbersym, plussym, minussym,
multsym, slashsym, oddsym, eqsym, neqsym,
lessym, leqsym,gtrsym, geqsym, lparentsym,
rparentsym, commasym, semicolonsym, periodsym, becomessym,
beginsym, endsym, ifsym, thensym, whilesym,
dosym, callsym, constsym, varsym, procsym,
writesym, readsym , elsesym } token_type;

typedef struct token{
  char text[MAX_TOKEN_LEN];
  int atribute;
}token;

typedef struct node{
  struct node *prev;
  struct node *next;
  token token;
}node;

// signitures
void chopFront(char *src,int len, int n);
void destroyLL(node *head);
void enqueue(node *ll, node *tail, token t);
char * fileToStr(char * fName);
int getToken(char ** codePtr, token *ret);
void initLL(node ** inHead, node ** inTail);
node * makeLexTable(char ** codePtr);
void printLexTable(node * head);
void printLexList(node * head);


int main(int argc, char** argv){

  if(argc != 2){
    printf("Usage: ./lex <PM0 source file>\n" );
    return ERROR;
  }

  // read file into a string
  char* code = NULL;
  code = fileToStr(argv[1]);
  if(code == NULL){
    return 0;
  }

  fprintf(stdout, "Source Program:%s\n", argv[1]);
  printf("%s\n", code);

  // build Lexeme table and Lexeme list
  node * lexTable = NULL;
  lexTable = makeLexTable(&code);

  if(lexTable != NULL){
    printLexTable(lexTable);
    printLexList(lexTable);
    destroyLL(lexTable);
  }
  // dealocate memory
  free(code);


}

/**
remove n characters from front of char array
*/
void chopFront(char *src,int len, int n){

  if(len-n+1 <= 0){
    strcpy(src,"");
  }
  else{
    char *temp = NULL;
    temp = calloc(1, len-n+1); // need to free?
    memmove(temp, (src)+n, len - n + 1);
    strcpy(src, temp);
    free(temp);
  }



}


void destroyLL(node * head){
  if (head -> next == NULL){
    free(head);
    return;

  }
  destroyLL(head->next);
  free(head);
}

/**

adds items to back of linked list
*/
void enqueue(node *ll, node *tail, token t){
  node * newNode = calloc(1, sizeof(node));

  newNode->token = t;
  newNode->next = tail;
  newNode->prev = tail->prev;

  tail->prev->next = newNode;
  tail->prev=newNode;

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

/*
find the next lexem, puts it in ret, and removes it from code

used pages 131 to 135 in compilers book

returns 0 on ok and -1 on error
*/
int getToken(char ** codePtr, token * ret){
  // the crux, might have tot make a custom tokenizer
  int start = 0;
  int current = 0;
  int codeLen = strlen(*codePtr);
  //int next = 1;

  token token;
  // init token to blank state
  strcpy(token.text,"");
  token.atribute = -1;
  int state = 0;
  int tokenLen = 0;
  char c;
  int found = 0;
  char * codeNew;// where code without token will go

  // repeat char processing untill return or failure occurs
  while(!found){

    switch(state){
      case 0:// start
        c = (*codePtr)[current++];

        if(c == '<') state = 1;
        else if(c == '=') state = 5;
        else if(c == '>') state = 6;
        else if((c >= 'a' && c <= 'z') || (c >='A' && c <='Z')){
          state = 10;
          tokenLen++;
        }
        else if((c >='0' && c <='9')){
          state = 13;
          tokenLen++;
        }
        else if(isspace(c)){
          state = 23;
        }
        else if(c == '/'){
          state = 25;
        }
        else if(c == '+'){
          state = 31;
        }
        else if(c == '-'){
          state = 32;
        }
        else if(c == '*'){
          state = 33;
        }
        else if(c == '('){
          state = 34;
        }
        else if(c == ')'){
          state = 35;
        }
        else if(c == ','){
          state = 36;
        }
        else if(c == ';'){
          state = 37;
        }
        else if(c == '.'){
          state = 38;
        }
        else if(c == ':'){
          state = 39;
        }
        else if(c == '\0'){
          // end of file reached
          return END;
        }
        else{
          printf("Error: Invalid symbol.");
          return ERROR;
        }


        break;

      case 1:// <
        c = (*codePtr)[current++];
        if(c == '=') {

          token.atribute = leqsym;
        }
        else if(c == '>'){

          token.atribute = neqsym;
        }
        else{
          current --;
          token.atribute = lessym;
        }
        found = 1;
        break;

      case 5:// =
        token.atribute = eqsym;
        found = 1;
        break;

      case 6:// >
        c = (*codePtr)[current++];
        if(c == '='){
          state = 7;
        }
        else{
          state = 8;
        }

      case 7: // >=
        token.atribute = geqsym;
        found = 1;
        break;

      case 8: // >
        token.atribute = gtrsym;
        current--;
        found = 1;
        break;

      case 10: // letter

        if (tokenLen > MAX_IDENTIFIER_LEN){
          // error
          printf("Error: Identifier too long.");
          return ERROR;
        }

        c = (*codePtr)[current++];


        // Debuging
        //printf("%s, %c, %d\n", *codePtr, c, current-1);

        if((c >= 'a' && c <= 'z') || (c >='A' && c <='Z') || (c >= '0' && c <= '9')){

          state = 10;
          tokenLen++;
          break;
        }
        else{
          state = 11;
        }


        break;

      case 11: // letters follwed by something else
        token.atribute = 0; // could be reserved word
        current--;
        found = 1;
        break;

      case 13: // numbers

        if(tokenLen > MAX_NUM_LEN){
          // error in input code
          printf("Error: Number too large.");
          return ERROR;
        }

        c = (*codePtr)[current++];
        if(c >= '0' && c <= '9'){
          tokenLen++;
        }
        else{ // non number found
          state = 20;
        }


        break;

      case 20: // number then something else
        // if its a not a letter we are fine
        if((c >= 'a' && c <= 'z') || (c >='A' && c <='Z')){
          //
          printf("Error: Invalid identifier.");
          return ERROR;
        }
        else{
          current--;
          found = 1;
          token.atribute = numbersym;
          break;
        }

      case 23: // whitespace

        // consume whitespace
        while(isspace((*codePtr)[current++])){
          // eat em up
        }

        state = 24;
        //break;

      case 24: // end of whitespace
        current--;

        chopFront(*codePtr , codeLen, current);

        //debug
        //printf("%s\n", *codePtr);

        return getToken(codePtr, ret);

      case 25: // /
        c=(*codePtr)[current++];

        if(c == '/'){
          state = 26;
        }
        else if(c == '*'){
          state = 28;
        }
        else{
          state = 30;
        }
        break;

      case 26: // single line comment
        do{
          c=(*codePtr)[current++];
        }while(!(c == '\0' || c == '\n' || c == '\r' ));
        /*if(c == '\0' || c == '\001'){ // end of program reached
          found = 1;
          break;

        }*/
        state = 27;

      case 27: // end of comment
        current;//  needed so compiler doesnt freak out

        chopFront(*codePtr , codeLen, current);

        // start from begining
        return getToken(codePtr, ret);

      case 28: // multi line comment
        while(!(c=(*codePtr)[current++] /*== '\0' || c*/ == '*' )){
          // ignore the comment
        }

        // '*' reached
        state = 29;

      case 29: // * encountered in multi line comment
        c = (*codePtr)[current++];

        if(c == '/'){
          state = 27;
        }
        else{
          state = 28;
        }
        break;

      case 30: // / folowed by not / or *
        current--;
        token.atribute = slashsym;
        found = 1;
        break;

      case 31: // +
        token.atribute = plussym ;
        found = 1;
        break;

      case 32: // -
        token.atribute = minussym;
        found = 1;
        break;

      case 33: // *
        token.atribute = multsym ;
        found = 1;
        break;

      case 34: // (
        token.atribute = lparentsym;
        found = 1;
        break;

      case 35: // )
        token.atribute = rparentsym;
        found = 1;
        break;

      case 36: // ,
        token.atribute = commasym ;
        found = 1;
        break;

      case 37: // ;
        token.atribute = semicolonsym;
        found = 1;
        break;
      case 38: // .
        token.atribute = periodsym;
        found = 1;
        break;

      case 39:// :
        c=(*codePtr)[current++];
        if (c=='='){
          state = 40;
        }
        else{
          printf("Error: Invalid token");
          return ERROR;
        }
        break;

      case 40: // :=
        token.atribute = becomessym;
        found = 1;
        break;
    }
  }

  // token found
  strncpy(token.text, *codePtr, current);
  token.text[current] = '\0';

  chopFront(*codePtr , codeLen, current);

  // if text check if reserved word
  if(token.atribute == 0){

    if(strcmp(token.text,"begin") == 0){
      token.atribute=beginsym;
    }
    else if(strcmp(token.text,"end") == 0){
      token.atribute = endsym;
    }
    else if(strcmp(token.text,"if") == 0){
      token.atribute = ifsym;
    }
    else if(strcmp(token.text,"then") == 0){
      token.atribute = thensym;
    }
    else if(strcmp(token.text,"while") == 0){
      token.atribute = whilesym;
    }
    else if(strcmp(token.text,"do") == 0){
      token.atribute = dosym;
    }
    else if(strcmp(token.text,"call") == 0){
      token.atribute = callsym;
    }
    else if(strcmp(token.text,"const") == 0){
      token.atribute = constsym;
    }
    else if(strcmp(token.text,"var") == 0){
      token.atribute = varsym;
    }
    else if(strcmp(token.text,"procedure") == 0){
      token.atribute = procsym;
    }
    else if(strcmp(token.text,"write") == 0){
      token.atribute = writesym;
    }
    else if(strcmp(token.text,"read") == 0){
      token.atribute = readsym;
    }
    else if(strcmp(token.text,"else") == 0){
      token.atribute = elsesym;
    }
    else{
      token.atribute = identsym;
    }
  }



  //free(codeNew);
  *ret = token;
  return OK;
}

void initLL(node ** inHead, node ** inTail){
  node *head = calloc(1,sizeof(node));
  node *tail = calloc(1,sizeof(node));

  head->next = tail;
  head->prev = NULL;
  //head->token.text =""
  tail->prev = head;
  tail->next = NULL;
  *inHead = head;
  *inTail = tail;
}

/*
makes a dynamicly sized list

@returns status
*/
node * makeLexTable(char ** codePtr){
  node * ll;
  //node * head;
  node * tail;
  initLL(&ll,&tail);

  token t;
  int status;

  status = getToken(codePtr,&t);
  while(status == OK){

    enqueue(ll,tail,t);
    status = getToken(codePtr,&t);
  }

  if (status == END){
    return ll;
  }
  else{
    //printf("Error: Invalid symbol");
    destroyLL(ll);
    return NULL;
  }
}

void printLexTable(node * head){
  fprintf(stdout, "\nLexeme Table:\n");
  fprintf(stdout, "lexeme\t\ttoken type\n");
  node *current =head->next;
  while(current->next != NULL ){

    fprintf(stdout, "%s\t\t%d\n", current->token.text, current->token.atribute);
    current =current->next;
  }
}

void printLexList(node * head){
  fprintf(stdout, "\nLexeme List:\n");
  node *current =head->next;
  while(current->next != NULL ){
    fprintf(stdout, "%d ", current->token.atribute);
    // If an identifier, print variable name
    if(current->token.atribute == 2){
      fprintf(stdout, "%s ", current->token.text);
    }
    // If number, print its ascii number value
    else if(current->token.atribute == 3){
      fprintf(stdout, "%s ", current->token.text);
    }
    current= current -> next;
  }
  printf("\n");

}
