#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#define BUF_SIZE 2048

void end(int x){
  printf("  --- end ---\n");
  exit(0);
}

int main(int argc,char *argv[])
{
  int fd,binded,port,i,recv,send,close2;
  struct sockaddr_in addr,addr2;
  socklen_t addrsize;
  char buf1[BUF_SIZE];

  if (signal(SIGINT,end) == SIG_ERR) {
    perror("signal");
    exit(1);
  }

  if(argc != 2) {
    printf("error\n");
    exit(1);
  }

  port = atoi(argv[1]);

  fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(fd<0){
    perror("socket");
    exit(1);
  }

  addr2.sin_family = AF_INET;
  addr2.sin_port = htons(port);
  addr2.sin_addr.s_addr = INADDR_ANY;

  binded = bind(fd, (struct sockaddr *)&addr2, sizeof(addr2));
  if(binded<0){
    perror("bind");
    close(fd);
    exit(1);
  }

  while (1) {
    for(i = 0;i < BUF_SIZE;i++) buf1[i] = '\0';

    addrsize = sizeof(addr);
    recv=recvfrom(fd,buf1,BUF_SIZE,0,(struct sockaddr *)&addr,&addrsize);
    if(recv<0){
      perror("recvfrom");
      close(fd); 
      exit(1);
    }
    printf("receive : %s",buf1);

    send=sendto(fd,buf1,recv,0,(struct sockaddr *)&addr,addrsize);
    if(send<0){
      perror("sendto");
      close(fd);
      exit(1);
    }
    printf("send    : %s\n",buf1);
  }

  if ((close2=close(fd))==-1){
    perror("close");
    exit(1);
  }

  return 0;

}