#include "scanner.h"
#include <stdio.h>

/*
 EBNF for gyre

 ident   = letter {letter | digit}.
 letter  = "A" | "B" | ... | "Z" | "a" | "b" | ... | "z".
 digit   = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9".
 number  = integer | real.
 integer = digit {digit} | digit {hexDigit} "H".
 real = digit {digit} "." {digit} [ScaleFactor].
 ScaleFactor = "E" ["+" | "-"] digit {digit}.
 hexDigit = digit | "A" | "B" | "C" | "D" | "E" | "F".
 string = """ {character} """ | digit {hexDigit} "X".

 qualification = seplist "in" ident ".".
 composition = ident "has" qidentlist ".".
 identity = ident "is" qidentlist ".".
 assertion = identity | composition | qualification.
 assertions = assertion {assertion}.
*/

int sym;

void assertions(void){
  int key;
  Get(&sym);
}


int main(int argc, char** argv) {
  int i;
  char c;
  FILE *f;

  _scanner();
  if (argc >= 2){
    for(i=1;i<argc;i++){
      printf("reading %s\n",argv[i]);
      f = fopen(argv[i], "r");
      if (f==NULL){
        printf("file not found\n");
      }else{
        Scanner_Init(f, 0);
	assertions();
      }
      fclose(f);
    }
  }
  else printf("no input\n");

  return 0;
}

