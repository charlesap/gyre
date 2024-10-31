// Copyright 2024 Gyre Authors, see AUTHORS
// MIT license, see LICENSE

#include "scanner.h"
#include "consolidator.h"
#include "generator.h"
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
char asid[IdLen];
char asidx[IdLen];
char module[IdLen];
char modulex[IdLen];

void Projection(void){
  char projid[IdLen];
  char projidx[IdLen];
  int which = sym;
  Get(&sym);
  if(sym != TO){
    Mark("no to"); 
  }else{
    Get(&sym);
    if(sym != IDENT){
      Mark("no identifier"); 
    }else{
      CopyId(projid,projidx);
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
        PutTo(asid,asidx);
	Get(&sym);
      }
    }
  }
}

void Composition(void){
    char compid[IdLen];
    char compidx[IdLen];
    CopyId(compid,compidx);
    Get(&sym);
    if(sym != IDENT){
      Mark("no type identifier"); 
    }else{
      Get(&sym);
      if(sym != IDENT){
        Mark("no instance identifier"); 
      }
      Get(&sym);
      while(sym == COMMA){
	Get(&sym);
	Get(&sym);
	Get(&sym);
      }
      if(sym == PERIOD){
          PutHas(asid,asidx);
          Get(&sym);
      }
    }
}

void Identity(void){

  Get(&sym);
  if(sym == PERIOD){
    printf(" IS\n");
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
          PutIs(asid,asidx);
          Get(&sym);
        }
      }
    }
  }
  if(sym == IS) Identity(); 
  if(sym == HAS) Composition(); 
}

void Qualification(void){
  char qualid[IdLen];
  char qualidx[IdLen];
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
      CopyId(qualid,qualidx);
      Get(&sym);
      if(sym != PERIOD){
        Mark("no period");
      }else{
        PutIn(qualid,qualidx);
        Get(&sym);
      }
    }
  }
}

void For(void){
      char forid[IdLen];
      char foridx[IdLen];
      Get(&sym);
      if(sym != IDENT){
	printf("no identifier"); 
      }else{
	CopyId(forid,foridx);
	CopyId(module,modulex);
	Get(&sym);
        if(sym != COLON){
	  printf("no colon");
	}else{
          PutFor(forid,foridx);
	  Get(&sym);
	}
      }
}


void Assertion(void){
    if (sym == FOR){
      For();
    }else if (sym == IDENT){
      CopyId(asid,asidx);
      Get(&sym);
      if (sym == IS){
        Identity();
      }else if(sym == HAS){
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

  module[0]=0;
  _scanner();
  _consolidator();
  _generator();
  if (argc >= 2){
    printf("Parsing\n");
    for(i=1;i<argc;i++){
      printf("from %s\n",argv[i]);
      Scanner_Init(argv[i]);
      sym = NUL;
      Assertions();
      Scanner_Close();
    }
    if(module[0]!=0){
      printf("Consolidating\n");
      Consolidate();
      Generator_Open(module);
      Prologue();
      Epilogue();
      Generator_Close();
    } 
  }
  else printf("no input\n");

  return 0;
}

