// Copyright 2024 Gyre Authors, see AUTHORS
// MIT license, see LICENSE

#include "generator.h"
#include "scanner.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h> 

FILE* W;








/*exported*/

void Generator_Open(char *f){
  char fn[IdLen+2];
  int i;
  for(i=0;f[i]!=0;i++) fn[i]=f[i];
  fn[i++]='.';
  fn[i++]='c';
  fn[i++]=0;
  printf("writing source to %s\n",fn);
  W = fopen(fn,"w+");
}

void Prologue(void){
  fprintf(W,"// begin generated source\n");
  fprintf(W,"#include <mpi.h>\n");
  fprintf(W,"#include <stdio.h>\n");
}

void Epilogue(void){
  fprintf(W,"int main(int argc, char** argv) {\n");
  fprintf(W,"    MPI_Init(NULL, NULL);\n");
  fprintf(W,"    int rank;\n");
  fprintf(W,"    int world;\n");
  fprintf(W,"    MPI_Comm_rank(MPI_COMM_WORLD, &rank);\n");
  fprintf(W,"    MPI_Comm_size(MPI_COMM_WORLD, &world);\n");
  fprintf(W,"    printf(\"Hello: rank %s, world: %s\\n\",rank, world);\n","%d","%d");
  fprintf(W,"    MPI_Finalize();\n");
  fprintf(W,"}\n");
  fprintf(W,"// end generated source\n");
}

void Generator_Close(void){
  fclose(W);
}

void _generator(void){
}
