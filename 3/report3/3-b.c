#include <stdio.h>
#include <pthread.h>
#define LENGTH 20

int bb_buf[LENGTH];
int count=0;
int position=0;

pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

int bb_get(void)
{
  int get;

  pthread_mutex_lock(&mutex);
  while(count == 0) {
    pthread_cond_wait(&empty,&mutex);    
  }
 
  get = bb_buf[position];
  position = (position+1)%LENGTH;
  count--;
  printf("bb_get %d\n",get);
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&full); 

  return get;
}

void bb_put(int n)
{
  pthread_mutex_lock(&mutex);
  while (count >= LENGTH){
    pthread_cond_wait(&full, &mutex);
  }

  bb_buf[(position+count)%LENGTH] = n;
  count++;
  printf("bb_put %d\n",n);
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&empty); 
}

void *func1(void *arg)
{
  int i=0;

  for(i = 0;i < *((int *)arg) ;i++) {
    bb_put(i);
  }

  pthread_exit(NULL);
}

void *func2(void *arg)
{
  int counter=0;

  for(counter = 0;counter < *((int *)arg) ;counter++) {
    bb_get();
  }

  pthread_exit(NULL);
}

int main(void) {

  int i=100;
  pthread_t thread1;
  pthread_t thread2;

  int create1=pthread_create(&thread1,NULL, &func1, &i);
  if(create1!=0){
      printf("error: pthread_create\n");
      return 1;
  }

    int create2=pthread_create(&thread2,NULL, &func2, &i);
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

  pthread_cond_destroy(&empty);
  pthread_cond_destroy(&full);

  return 0;
}

