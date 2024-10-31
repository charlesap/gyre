// Copyright 2024 Gyre Authors, see AUTHORS
// MIT license, see LICENSE

#include <stdbool.h>
#include <stdlib.h>

#define IdInNode 8
#define InternSize (1<<24)

#define Prep 1
#define ForFlag 2
#define IsFlag 4
#define InFlag 8
#define HasFlag 16
#define ToFlag 32

struct head{
  char * id;
  struct element * next;
};

struct element{
  char flag;
  uint32_t a;
  uint16_t b;
  char c;
  struct element * next;
  struct element * ref;
};

struct node {
  int count;
  char * id[IdInNode];
  struct element * data[IdInNode];
  struct node * child[IdInNode];
  bool leaf;
};

void PutFor(char * forid, char * foridx);
void PutIs(char * asid, char * asidx);
void PutIn(char * qualid, char * qualidx);
void PutHas(char * asid, char * asidx);
void PutTo(char * asid, char * asidx);
void Consolidate(void);
void _consolidator(void);
