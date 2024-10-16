// Copyright 2024 Gyre Authors, see AUTHORS
// MIT license, see LICENSE

#include "generator.h"
#include "scanner.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h> 

FILE* W;







void o(char *s){
  fputs(s,W);
  fputc('\n',W);
}

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
  o("// begin generated source");
  o("#include <mpi.h>");
  o("#include <stdio.h>");
}

void Epilogue(void){
  o("int main(int argc, char** argv) {");
  o("    MPI_Init(NULL, NULL);");
  o("    int rank;");
  o("    int world;");
  o("    MPI_Comm_rank(MPI_COMM_WORLD, &rank);");
  o("    MPI_Comm_size(MPI_COMM_WORLD, &world);");
  o("    printf(\"Zero: rank %d, world: %d\\n\",rank, world);fflush(stdout);");
  o("    MPI_Barrier(MPI_COMM_WORLD);");
  o("    printf(\"One: rank %d, world: %d\\n\",rank, world);fflush(stdout);");
  o("    MPI_Finalize();");
  o("}");
  o("// end generated source");
}

void Generator_Close(void){
  fclose(W);
}

void _generator(void){
}
