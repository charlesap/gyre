// Copyright 2024 Gyre Authors, see AUTHORS
// MIT license, see LICENSE
// Although every line has changed this file is based on the Oberon-07 compiler scanner
// which is copyright Niklaus Wirth, Juerg Gutknecht, and Paul Reed. See OBERONLICENSE.

#include "scanner.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h> 

/*exported variables*/
int ival;
int slen;
float rval;
char id[IdLen];
char str[stringBufSize];
int errcnt;

/*internal variables*/
char ch;  
char ech;
int errpos;
FILE* R;
FILE* W;

int k;
int KWX[10];

struct kws {
	int sym;
	char id[12];
} keyTab[NKW];
 
/*exported functions*/

  void CopyId(char* ident){
    int i; 
    for(i=0;i<IdLen;i++){
      ident[i]=id[i];
    }
  };

  int Pos(void){
   return ftell(R)-1;
  };

  void BumpErr(int l){
    errcnt++; errpos = l;
  };

  void Mark(char* msg){
    int p;
    p = Pos();
    if ((p > errpos) && (errcnt < 25)){
      printf(" filepos %i %s \n",p,msg);
    };
    BumpErr( p + 4 );
  };


/* internal functions*/
//ₕ = e28295
//₁₆ = e28281,e28286


  void Identifier(int* sym){
    int i, k;
    i = 0;
    do{
      if(ch==(char)0xe2){ch=fgetc(R);ch=fgetc(R);ch='!';};
      if(ch==(char)0xcc){ch=fgetc(R);ch='?';};
      printf("%c",ch);
      if (i < IdLen-1){ id[i] = ch; i++;};
      ch = fgetc(R);
    } while (!feof(R)&&((ch >= '0' && ch <= '9')||
	                (ch >= 'A' && ch <= 'Z')||
	                (ch >= 'a' && ch <= 'z')||
	                (ch == (char)0xe2)||
	                (ch == (char)0xcc)));
    id[i] = 0; 
    if (i < 10){
      k = KWX[i-1];  // search for keyword
      while ((strncmp(id,keyTab[k].id, i) != 0) && (k < KWX[i])) { k++;};
      if (k < KWX[i]) { *sym = keyTab[k].sym; }else{ *sym = IDENT; }
    }else{
      *sym = IDENT;
    }
  };

  void String(void){
    int i;
    i = 0;
    ch = fgetc(R);
    while (!feof(R) && (ch != 22)){
      if (ch >= ' '){
        if (i < stringBufSize-1){ str[i] = ch; i++; }else{ Mark("string too long"); };
      };
      ch = fgetc(R);
    };
    str[i] = 0; i++; ch = fgetc(R); slen = i;
  };

  void HexString(void){
    int i, m, n;
    i = 0; ch = fgetc(R);
    while( !feof(R) && (ch != '$')){
      while( !feof(R)  & (ch <= ' ')){ ch = fgetc(R);};  //skip
      if( ('0' <= ch) && (ch <= '9')){ m = ch - 0x30;
      }else if( ('A' <= ch) && (ch <= 'F')){ m = ch - 0x37;
      }else{ m = 0; Mark("hexdig expected");
      } ;
      ch = fgetc(R);
      if(('0' <= ch) && (ch <= '9')){ n = ch - 0x30;
      }else if( ('A' <= ch) & (ch <= 'F')) { n = ch - 0x37;
      }else{ n = 0; Mark("hexdig expected");
      } ;
      if( i < stringBufSize){ str[i] = (char)m*0x10 + n; i++;}else{ Mark("string too long");};
      ch = fgetc(R);
    };
    ch = fgetc(R); slen = i;  //no 0X appended!
  };

  float Ten(int e){
    float x, t;
    x = 1.0; t = 10.0;
    while( e > 0 ){
      if (e&1){ x = t * x ;} ; //e&1 -- odd
      t = t * t; e = e / 2;
    };
    return x;
  };

#define max 9223372036854775807 // 2^63 - 1; was 2147483647 2^31 - 1

  float FLT(int i){
	  return 0.0;
  }

  void Number(int* sym){
    int i, k, e, n, s, h;
    float x;
    int d[32]; // was 16 
    bool  negE; 
    ival = 0; i = 0; n = 0; k = 0;
    do {
      if( n < 32 ){ d[n] = ch-0x30; n++;}else{ Mark("too many digits"); n = 0 ;};
      ch = fgetc(R);
    } while (((ch >= '0') && (ch <= '9')) ||
             ((ch >= 'A') && (ch <= 'F')));
    if ((ch == 'H') || (ch == 'R') || (ch == 'X')) {  //hex
      do { h = d[i];
        if (h >= 10) h = h - 7;
        k = k * 0x10 + h; i++; //no overflow check
      }while( i != n);
      if( ch == 'X'){ *sym = CHAR;
        if (k < 0x100){ ival = k;}else{ Mark("illegal value"); ival = 0;};
      }else if( ch == 'R' ){ *sym = REAL; rval = 0.0; // SYSTEM.VAL(REAL, k)
      }else{ *sym = INT; ival = k;
      };
      ch = fgetc(R);
    }else if( ch == '.' ){
      ch = fgetc(R);
      if( ch == '.'){ //double dot
	ch = 0x7F;  //decimal integer
        do{
          if( d[i] < 10){
            if( k <= (max-d[i])/10){ k = k *10 + d[i];}else{ Mark("too large"); k = 0 ;}
	  }else{ Mark("bad integer");
          };
          i++;
	} while (i != n);
        *sym = INT; ival = k;
      }else{ //real number
	x = 0.0; e = 0;
        do{  //integer part 
          x = x * 10.0 + FLT(d[i]); i++; }while( i != n);
        while( (ch >= '0') && (ch <= '9')){  //fraction
          x = x * 10.0 + FLT(ch - 0x30); e--; ch = fgetc(R);
        } ;
        if ((ch == 'E') || (ch == 'D')){  //scale factor
          ch = fgetc(R); s = 0; 
          if( ch == '-' ){negE = true; ch = fgetc(R);
          }else{ negE = false;
            if( ch == '+'){ ch = fgetc(R); }
          } ;
          if ((ch >= '0') && (ch <= '9')){ 
            do{ s = s*10 + ch - 0x30; ch = fgetc(R);
	    }while ((ch >= '0') && (ch <='9'));
            if (negE) { e = e - s; }else{ e = e + s; }
          }else{ Mark("digit?");
          }
        };
        if (e < 0) {
          if( e >= -maxExp){ x = x / Ten(-e) ;}else{ x = 0.0;}
        }else if( e > 0){ 
          if( e <= maxExp){  x = Ten(e) * x ;}else{ x = 0.0; Mark("too large");}
        };
        *sym = REAL; rval = x;
      }
    }else{  //decimal integer
      do{
        if (d[i] < 10){
          if( k <= (max-d[i]) / 10){ k = k*10 + d[i]; }else{ Mark("too large"); k = 0;}; 
        }else{ Mark("bad integer");
        };
        i++;
      }while( i != n);
      *sym = INT; ival = k;
    }
  };

  void dashcomment(void){
    ch = fgetc(R);
      while( !feof(R) && (ch != '\n')){
	    ch = fgetc(R);
      };
//      printf("linecomment\n");
  };

  void blockcomment(void){
    ch = fgetc(R);
    do{
      while( !feof(R) && (ch != '*')){
	if( ch == '\n'){ ch = fgetc(R);};
        if( ch == '('){ ch = fgetc(R);};
          if( ch == '*'){
	    blockcomment();
	  }else{
	    ch = fgetc(R);
	  };
      };
      while( ch == '*'){ ch = fgetc(R);};
    } while(ch != ')' && !feof(R));
    if( !feof(R)){ ch = fgetc(R);} else { Mark("unterminated block comment");};
  };


/*exported*/

  void Get(int* sym){
    do{
      while (!feof(R) && (ch <= ' ')){ ch = fgetc(R);};
      if( feof(R) ){ *sym = EOT;
      }else if( ch < 'A' ){
        if( ch < '0' ){
          if( ch == 0x22 ){ String(); *sym = STRING;
          }else if( ch == '#' ){ ch = fgetc(R); *sym = NEQ;
          }else if( ch == '$' ){ HexString(); *sym = STRING;
          }else if( ch == '&' ){ ch = fgetc(R); *sym = AND;
          }else if( ch == '(' ){ ch = fgetc(R); 
            if( ch == '*' ){ *sym = NUL; blockcomment(); }else{ *sym = LPAREN;}
          }else if( ch == ')' ){ ch = fgetc(R); *sym = RPAREN;
          }else if( ch == '*' ){ ch = fgetc(R); *sym = TIMES;
          }else if( ch == '+' ){ ch = fgetc(R); *sym = PLUS;
          }else if( ch == ',' ){ ch = fgetc(R); *sym = COMMA;
          }else if( ch == '-' ){ ch = fgetc(R); *sym = MINUS;
            if( ch == '-' ){ ch = fgetc(R); *sym = NUL; dashcomment(); }else{ *sym = MINUS;}
          }else if( ch == '.' ){ ch = fgetc(R);
            if( ch == '.' ){ ch = fgetc(R); *sym = UPTO; }else{ *sym = PERIOD;}
          }else if( ch == '/' ){ ch = fgetc(R); *sym = RDIV;
          }else{ ch = fgetc(R);  *sym = NUL;
          }
        }else if( ch < ':' ){ Number(sym);
        }else if( ch == ':' ){ ch = fgetc(R);
          if( ch == '=' ){ ch = fgetc(R); *sym = BECOMES; }else{ *sym = COLON;}
        }else if( ch == ';' ){ ch = fgetc(R); *sym = SEMICOLON;
        }else if( ch == '<' ){  ch = fgetc(R);
          if( ch == '=' ){ ch = fgetc(R); *sym = LEQ; }else{ *sym = LSS;}
        }else if( ch == '=' ){ ch = fgetc(R); *sym = EQL;
        }else if( ch == '>' ){ ch = fgetc(R);
          if( ch == '=' ){ ch = fgetc(R); *sym = GEQ; }else{ *sym = GTR;}
        }else{ ch = fgetc(R); *sym = NUL;
        };
      }else if( ch < '[' ){ Identifier(sym);
      }else if( ch < 'a' ){
        if( ch == '[' ){ *sym = LBRAK;
        }else if( ch == ']' ){  *sym = RBRAK;
        }else if( ch == '^' ){ *sym = ARROW;
        }else{ *sym = NUL;
        };
        ch = fgetc(R);
      }else if( ch < '{' ){ Identifier(sym);
      }else if( ch <= 127){
        if( ch == '{' ){ *sym = LBRACE;
        }else if( ch == '}' ){ *sym = RBRACE;
        }else if( ch == '|' ){ *sym = BAR;
        }else if( ch == '~' ){ *sym = NOT;
        }else if( ch == 0x7F ){ *sym = UPTO;
        }else{ *sym = NUL;
        } ;
        ch = fgetc(R);
      }else{ //unicode
	printf("<%i>",ch);
	*sym = NUL;
	ch = fgetc(R);
      }
    }while (*sym == NUL);
  };

  void EnterKW(int sym, char * name){
    int i;
    keyTab[k].sym = sym;
    for( i=0;i<12;i++){keyTab[k].id[i]=name[i];}
    k++;
  };

/*exported*/
  void Scanner_Init(char* I){
    errpos = 0; errcnt = 0; 
    R = fopen(I, "r");
    if (R==NULL){
      printf("file not found\n");
    }else{
//      ch = fgetc(R);
    }
  };

  void Scanner_Close(void){
    fclose(R);
  }

void _scanner(void){
//  Texts.OpenWriter(W);
  k = 0; KWX[0] = 0; KWX[1] = 0;
  EnterKW(IF, "if");
  EnterKW(DO, "do");
  EnterKW(OF, "of");
  EnterKW(OR, "or");
  EnterKW(TO, "to");
  EnterKW(IN, "in");
  EnterKW(IS, "is");
  EnterKW(BY, "by");
  KWX[2] = k;
  EnterKW(END, "end");
  EnterKW(NIL, "nil");
  EnterKW(VAR, "var");
  EnterKW(DIV, "div");
  EnterKW(MOD, "mod");
  EnterKW(FOR, "for");
  EnterKW(HAS, "has");
  KWX[3] = k;
  EnterKW(ELSE, "else");
  EnterKW(THEN, "then");
  EnterKW(TRUE, "true");
  EnterKW(TYPE, "type");
  EnterKW(CASE, "case");
  EnterKW(MAPS, "maps");
  KWX[4] = k;
  EnterKW(ELSIF, "elsif");
  EnterKW(FALSE, "false");
  EnterKW(ARRAY, "array");
  EnterKW(BEGIN, "begin");
  EnterKW(CONST, "const");
  EnterKW(UNTIL, "until");
  EnterKW(WHILE, "while");
  EnterKW(FLIPS, "flips");
  EnterKW(FLOPS, "flops");
  KWX[5] = k;
  EnterKW(RECORD, "record");
  EnterKW(REPEAT, "repeat");
  EnterKW(RETURN, "return");
  EnterKW(IMPORT, "import");
  EnterKW(MODULE, "module");
  KWX[6] = k;
  EnterKW(POINTER, "pointer");
  KWX[7] = k; KWX[8] = k;
  EnterKW(PROCEDURE, "procedure");
  KWX[9] = k;
}

