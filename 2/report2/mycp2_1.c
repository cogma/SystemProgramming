#include <stdio.h>
#include <stdlib.h>
#include <errno.h> /* perror(3) */
#include <sys/time.h>
#include <sys/stat.h>

struct stat buf1;
struct stat buf2;

void mycp2(char *file1,char *file2)
{
  FILE *fp1;
  FILE *fp2;
  int read1;
  int write2;
  char buf[1];
  struct timeval t0;
  struct timeval t1;

  gettimeofday(&t0, NULL);

  if((stat(file1,&buf1)==0)&&(stat(file2,&buf2)==0)){
    if(buf1.st_ino==buf2.st_ino){
      printf("mycp: `%s' と `%s' は同じファイルです\n", file1,file2);
      exit(1);
    }
  }

  fp1 = fopen(file1,"r");
  if(fp1==NULL){
    perror(file1);
    exit(1);
  }
  fp2 = fopen(file2,"w");
  if(fp2== NULL) {
    perror(file2);
    fclose(fp1);
    exit(1);
  }

  while((read1=fread(buf,sizeof(char),1,fp1))>0){
    if((write2=fwrite(buf,sizeof(char),read1,fp2))!=read1){
	perror(file2);
	fclose(fp1);
	fclose(fp2);
	exit(1);
      }
  }

  fclose(fp1);
  fclose(fp2);

  gettimeofday(&t1, NULL);

  if (t1.tv_usec < t0.tv_usec) {
    printf("%ld.%06ld s\n", t1.tv_sec - t0.tv_sec - 1, 1000000 + t1.tv_usec - t0.tv_usec);
  }
  else {
    printf("%ld.%06ld s\n", t1.tv_sec - t0.tv_sec, t1.tv_usec - t0.tv_usec);
  }
}

int main(int argc,char *argv[])
{
  if(argc != 3)
  {
    printf("error\n");
    exit(1);
  }
  mycp2(argv[1],argv[2]);

  return 0;
}
