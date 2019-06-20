#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <malloc.h>

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


void *func(void *arg)
{
  int i=0;

  for(i = 0;i < 100 ;i++) {
    arg=btree_insert(rand()%100, arg);
  }

  pthread_exit(NULL);
}

int main(void) {

  pthread_t thread1;
  pthread_t thread2;
  tr = btree_create();

  int create1=pthread_create(&thread1,NULL, &func, tr);
  if(create1!=0){
      printf("error: pthread_create\n");
      return 1;
  }

  int create2=pthread_create(&thread2,NULL, &func, tr);
  if(create2!=0){
      printf("error: pthread_create\n");
      return 1;
  }

  int join1=pthread_join(thread1, NULL);
  if(join1 !=0){
      printf("error: pthread_join\n");
      return 1;
  }
  int join2=pthread_join(thread2, NULL);
  if(join2 !=0){
      printf("error: pthread_join\n");
      return 1;
  }

  btree_dump(tr);

  btree_destroy(tr);

  printf("%d\n",count);

  return 0;
}
