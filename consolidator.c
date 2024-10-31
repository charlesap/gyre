// Copyright 2024 Gyre Authors, see AUTHORS
// MIT license, see LICENSE

#include "scanner.h"
#include "consolidator.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h> 
#include <stdlib.h>


struct node * new(bool l){
//  printf(" new node\n");
  struct node * n = (struct node *)malloc(sizeof(struct node));
  if(n==NULL){
    printf("out of memory\n");
    exit(1);
  };
  n->count=0;
  n->leaf=l;
  for(int i=0;i<IdInNode;i++){
    n->child[i] = NULL;
  };
  return n;
}

char *ntrn;
int ntrnf;

void split(struct node *p, int x){
  struct node *c = p->child[x];
  struct node *n = new(c->leaf);
  n->count = IdInNode / 2 - 1;
  for(int i=0;i<IdInNode -1; i++){
    n->id[i]=c->id[i+1];
  }
  if(!c->leaf) {
    for(int i = 0; i < IdInNode/2; i++) {
      n->child[i] = c->child[i + IdInNode/2];
    }
  }
  c->count = IdInNode/2 - 1;
  for (int i = p->count; i > x; i--) {
    p->child[i + 1] = p->child[i];
  }
  p->child[x + 1] = n;
  for (int i = p->count - 1; i >= x; i--) {
    p->id[i + 1] = p->id[i];
  };
  for(int j=0;j<IdLen;j++){
    p->id[x+j] = c->id[IdLen/2 - 1 + j];
  }
  p->count++;
}

bool insertx(struct node * n, struct element * el, char * si){
//  printf(" existing node\n");
  int i=n->count-1;
  if(n->leaf){
//    printf(" leaf\n");
//    if(n==0) printf(" n is null\n");
    while((i >= 0) && (n->id[i] != 0) && strncmp(n->id[i],si,IdLen)>0){
        n->id[i+1]=n->id[i];
        n->data[i+1]=n->data[i];
        i--;  
    }
    n->id[i+1]=si;
    n->data[i+1]=el;
    n->count++;
//    printf("  %s %d\n",n->id[i+1],n->count);
  }else{
//    printf(" interior\n");
    while(i >= 0 && (strncmp(n->id[i],si,IdLen)>0)){
	i--;
    }
    i++;
  }
  if((n->child[i]!=0)&&(n->child[i]->count == IdInNode - 1)){
//    printf(" now full\n");
    split(n,i);
    if(strncmp(n->id[i],si,IdLen)<0){
      i++;
    }
    insertx(n->child[i],el,si);
  }
  return true;
}


bool exists(char * n){
  return false;
}

bool insert(struct node **r, struct element * el, char * s) {
  struct node *n = *r;
  char * si;
  bool ok;

  if(exists(s)){
    printf("already exists %s\n",s);
    ok = false;
  }else{  
//    printf("interning %s\n",s);
    if(strnlen(s,IdLen) >= (InternSize - 1)){
      printf("intern string buffer exceeded %s\n",s);
      ok = false;
    }else{
//      printf("recording %s\n",s);
      si = &ntrn[ntrnf];
      for(int j=0;(j<IdLen && s[j]!=0);j++){
        ntrn[ntrnf++]=s[j]; 
      }
      ntrn[ntrnf++]=0;
//      printf("inserting %s\n",s);
      if (n == NULL) {
          *r = new(true);
	  (*r)->id[0] = si;
	  (*r)->data[0] = el;
          (*r)->count = 1;
          ok = true;
      } else {
          if (n->count == IdInNode - 1) {
              struct node *nr = new(false);
              nr->child[0] = n;
              split(nr, 0);
              *r = nr;
          }
          ok = insertx(*r, el, s);
      }
    }
  }
  return ok;
}

void scan(struct node *n) {
    if (n != NULL) {
        int i;
        for (i = 0; i < n->count; i++) {
            scan(n->child[i]);
            printf("%s ", n->id[i]);
        }
        scan(n->child[i]);
    }
}

struct node * root;

int test() {
    bool ok;


    ok = insert(&root, 0, "banana");
    ok = insert(&root, 0, "aardvark");
    ok = insert(&root, 0, "tomato");
    ok = insert(&root, 0, "aardvark");
    ok = insert(&root, 0, "ostrich");
    ok = insert(&root, 0, "boat");
    ok = insert(&root, 0, "tree");

    printf("contents: ");
    scan(root);
    printf("\n");

    return 0;
}









void PutFor(char * forid, char * foridx){
  bool ok = insert(&root, 0, forid);
	printf(" FOR: %s%s\n",forid,foridx);
}

void PutIs(char * asid, char * asidx){
//  bool ok = insert(&root, 0, asid);
        printf(" IS: %s%s\n",asid,asidx);
}

void PutIn(char * qualid, char * qualidx){
//  bool ok = insert(&root, 0, qualid);
        printf(" IN: %s%s\n",qualid,qualidx);
}

void PutHas(char * asid, char * asidx){
//  bool ok = insert(&root, 0, asid);
        printf(" HAS: %s%s\n",asid,asidx);
}

void PutTo(char * asid, char * asidx){
//  bool ok = insert(&root, 0, asid);
        printf(" TO: %s%s\n",asid,asidx);
}

void Consolidate(void){
  printf("contents: ");
  scan(root);
  printf("\n");
}

void _consolidator(void){
  ntrn = (char *)malloc(InternSize);
  ntrnf = 0;
  root = NULL;
//  test();
}
