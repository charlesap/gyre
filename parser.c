// Copyright 2024 Gyre Authors, see AUTHORS
// MIT license, see LICENSE

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
 projector = "maps" | "flips" | "flops".

 Projection = ident projector ident {"and" ident}.
 qidentlist = ident {"," ident} ".". 
 Qualification = seplist "in" ident ".".
 Composition = ident "has" qidentlist ".".
 Identity = ident "is" [ | qidentlist { ident "is" qidentlist} ] ".".
 for = "for" ident ".".
 Assertion = For | Identity | Composition | Qualification | Projection.
 Assertions = Assertion {Assertion}.
*/

int sym;

void Projection(void){
  int which = sym;
  Get(&sym);
  if(sym != TO){
    Mark("no to"); 
  }else{
    Get(&sym);
    if(sym != IDENT){
      Mark("no identifier"); 
    }else{
      Get(&sym);
      while(sym == COMMA){
        Get(&sym);
        if(sym != IDENT){
  	Mark("no identifier after comma");
        }else{
          Get(&sym);
        }
      };
      if(sym != PERIOD){
	      printf("#%i#",sym);
        Mark("no period");
      }else{
        printf("\n TO\n");
        Get(&sym);
      }
    }
  }
}

void Composition(void){
    Get(&sym);
    if(sym != IDENT){
      Mark("no type identifier"); 
    }else{
      Get(&sym);
      if(sym != IDENT){
        Mark("no instance identifier"); 
      }
      Get(&sym);
      printf("==%i=>",sym);
      while(sym == COMMA){
	Get(&sym);
	Get(&sym);
	Get(&sym);
      }
      if(sym == PERIOD){
          printf("\n HAS\n");
          Get(&sym);
      }
    }
}

void Identity(void){

  Get(&sym);
  if(sym == PERIOD){
    printf("\n IS\n");
    Get(&sym);
  }else{
    if(sym != IDENT){
      Mark("no identifier"); 
    }else{
      Get(&sym);
      if(sym == IDENT){
	Get(&sym);
      }else{
        while(sym == COMMA){
          Get(&sym);
          if(sym != IDENT){
         Mark("no identifier after comma");
          }else{
            Get(&sym);
          }
        }
      
        if(sym != PERIOD){
          Mark("no period");
        }else{
          printf("\n IS\n");
          Get(&sym);
        }
      }
    }
  }
  if(sym == IS) Identity(); 
  if(sym == HAS) Composition(); 
}

void Qualification(void){
//	printf("{");
  while(sym == BAR){
    Get(&sym);
    if(sym != IDENT){
      Mark("no identifier after bar");
    }else{
      Get(&sym);
    }
  }
  if(sym != IN){
    Mark("no in");
  }else{
    Get(&sym);
    if(sym != IDENT){
      Mark("no identifier after in");
    }else{
      Get(&sym);
      if(sym != PERIOD){
        Mark("no period");
      }else{
        printf("\n IN\n");
        Get(&sym);
      }
    }
  }
//	printf("}");
}

void For(void){
      Get(&sym);
      if(sym != IDENT){
	printf("no identifier"); 
      }else{
	Get(&sym);
        if(sym != COLON){
	  printf("no colon");
	}else{
	  printf("\n FOR\n");
	  Get(&sym);
	}
      }
}


void Assertion(void){
//	printf("[%i]",sym);
    if (sym == FOR){
      For();
    }else if (sym == IDENT){
      Get(&sym);
      if (sym == IS){
        Identity();
      }else if(sym == HAS){
        printf("--->");
        Composition();
      }else if(sym == BAR){
        Qualification();
      }else if((sym == MAPS)||(sym == FLIPS)||(sym == FLOPS)){
        Projection();
      }else{
        Mark("no is/has/in");
        sym = EOT;
      }
    }else{
      sym = EOT;
      Mark("assertion not recognized.");
    };
//    if(sym != EOT){Get(&sym);}
}

void Assertions(void){
  Get(&sym);
  while( sym != EOT ){
    Assertion();
  }
}

int main(int argc, char** argv) {
  int i;
  char c;
  FILE *f;

  _scanner();
  if (argc >= 2){
    for(i=1;i<argc;i++){
      printf("reading %s\n",argv[i]);
      Scanner_Init(argv[i]);
      sym = NUL;
      Assertions();
      Scanner_Close();
    }
  }
  else printf("no input\n");

  return 0;
}

