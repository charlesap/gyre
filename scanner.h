#include <stdio.h>

#define IdLen 32
#define NKW 34
#define maxExp 38 
#define stringBufSize 256
#define NUL 0
#define TIMES 1 
#define RDIV 2 
#define DIV 3 
#define MOD 4
#define AND 5 
#define PLUS 6 
#define MINUS 7 
#define OR 8 
#define EQL 9
#define NEQ 10 
#define LSS 11 
#define LEQ 12 
#define GTR 13 
#define GEQ 14
#define IN 15 
#define IS 16 
#define ARROW 17 
#define PERIOD 18
#define CHAR 20 
#define INT 21 
#define REAL 22 
#define FALSE 23 
#define TRUE 24
#define NIL 25 
#define STRING 26 
#define NOT 27 
#define LPAREN 28 
#define LBRAK 29
#define LBRACE 30 
#define IDENT 31
#define IF 32 
#define WHILE 34 
#define REPEAT 35 
#define CASE 36 
#define FOR 37
#define COMMA 40 
#define COLON 41 
#define BECOMES 42 
#define UPTO 43 
#define RPAREN 44
#define RBRAK 45 
#define RBRACE 46 
#define THEN 47 
#define OF 48 
#define DO 49
#define TO 50 
#define BY 51 
#define SEMICOLON 52 
#define END 53 
#define BAR 54
#define ELSE 55 
#define ELSIF 56 
#define UNTIL 57 
#define RETURN 58
#define ARRAY 60 
#define RECORD 61 
#define POINTER 62 
#define CONST 63 
#define TYPE 64
#define VAR 65 
#define PROCEDURE 66 
#define BEGIN 67 
#define IMPORT 68 
#define MODULE 69 
#define EOT 70

extern char id[IdLen];
extern int ival;
extern int slen;
extern float rval;
extern char str[stringBufSize];
extern int errcnt;

void CopyId(char* ident);
int Pos(void);
void BumpErr(int l);
void Mark(char* msg);
void Get(int* sym);
void Scanner_Init(FILE* T, int pos);
void _scanner(void);


