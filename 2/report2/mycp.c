#include <sys/types.h> /* open(2) */
#include <sys/stat.h> /* open(2) */
#include <fcntl.h> /* open(2) */
#include <unistd.h> /* read(2) */
#include <errno.h> /* perror(3) */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct stat buf1;
struct stat buf2;

void mycp(char *file1,char *file2)
{
  int fd1;
  int fd2;
  int read1;
  int write2;
  int close2;
  char buf[8192];

  if((stat(file1,&buf1)==0)&&(stat(file2,&buf2)==0)){
    if(buf1.st_ino==buf2.st_ino){
      printf("mycp: `%s' と `%s' は同じファイルです\n", file1,file2);
      exit(1);
    }
  }

  fd1 = open(file1,O_RDONLY);
  if(fd1==-1){
    perror(file1);
    exit(1);
  }
  fd2 = open(file2,O_CREAT|O_WRONLY|O_TRUNC,0666);
  if(fd2 == -1) {
    perror(file2);
    close(fd1);
    exit(1);
  }

  while((read1=read(fd1,buf,8192))>0){
    if((write2=write(fd2,buf,read1))==-1){
	perror(file2);
	close(fd1);
	if ((close2=close(fd2))==-1){
	  perror(file2);
	  exit(1);
	}
	exit(1);
      }
    lseek(fd1,write2-read1,SEEK_CUR);
  }

  close(fd1);
  if ((close2=close(fd2))==-1){
    perror(file2);
    exit(1);
  }
}

int main(int argc,char *argv[])
{
  if(argc != 3)
  {
    printf("error\n");
    exit(1);
  }
  mycp(argv[1],argv[2]);

  return 0;
}
