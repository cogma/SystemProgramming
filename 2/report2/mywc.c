#include <sys/types.h> /* open(2) */
#include <sys/stat.h> /* open(2) */
#include <fcntl.h> /* open(2) */
#include <unistd.h> /* read(2) */
#include <errno.h> /* perror(3) */
#include <stdlib.h>
#include <stdio.h>

void mywc(char *file)
{
  int fd;
  int read1;
  int line=0;
  int word=0;
  int byte=0;
  int i;
  char buf[8192];

  fd = open(file,O_RDONLY);
  if(fd==-1){
    perror(file);
    exit(1);
  }

  while((read1=read(fd,buf,8192))>0){
    for(i=0;i<read1;i++){
      if(buf[i]=='\n'){line++;if(buf[i+1]!='\n')word++;;}
      else if(buf[i]==' ') {word++;}
    }
    byte = byte + read1;
  }
  printf("%d lines, %d words, %d bytes\n", line, word, byte);

  close(fd);
}

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    printf("error\n");
    exit(1);
  }
  mywc(argv[1]);

  return 0;
}
