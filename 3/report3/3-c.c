#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#define _SCHED_H 1
#define __USE_GNU 1
#include <bits/sched.h>
#define STACK_SIZE 4096

typedef struct tree tnode;

struct tree {
    int value;
    tnode *left;
    tnode *right;
};

tnode *tr;
int count=0;

tnode *btree_create(void)
{
  tnode *t;
  
  t=malloc(sizeof(tnode));
  if(t!=NULL){
    t->left=NULL;
    t->right=NULL;
  }

  return t;
}

int btree_isempty(tnode *t)
{
  int s=0;

  if((t->left==NULL)&&(t->right==NULL))
    s=1;

  return s;
}

tnode *btree_insert(int v, tnode *t)
{
  if(btree_isempty(t)){
    t->value=v;
    t->left=btree_create();
    t->right=btree_create();
  }

  else if(v < t->value)
    t->left=btree_insert(v,t->left);
  
  else
    t->right=btree_insert(v,t->right);

  return t;
}

void btree_destroy(tnode *t)
{
  if(!btree_isempty(t)){
    btree_destroy(t->left);
    btree_destroy(t->right);
    free(t);
  }
}

void btree_dump(tnode *t)
{
  if(!(btree_isempty(t))){
    btree_dump(t->left);
    printf("%d\n", t->value);
    btree_dump(t->right);
    count++;
  }
}

int func(void *arg)
{
  int i=0;

  for(i = 0;i < 100 ;i++) {
    arg=btree_insert(rand()%100, arg);
  }

  return 0;
}

int main() {
  int m=0;
  int n=0;
  void *child_stack1 = malloc(STACK_SIZE);
  void *child_stack2 = malloc(STACK_SIZE);
  tr = btree_create();

  m=clone(&func, child_stack1+STACK_SIZE, CLONE_SIGHAND|CLONE_FS|CLONE_VM|CLONE_FILES, tr);
  if(m==-1){
    perror("clone");
    exit(1);
  }
  
  n=clone(&func, child_stack2+STACK_SIZE, CLONE_SIGHAND|CLONE_FS|CLONE_VM|CLONE_FILES, tr);
  if(n==-1){
    perror("clone");
    exit(1);
  }

  sleep(2);
  
  btree_dump(tr);
  printf("%d\n",count);

  return 0;
}
