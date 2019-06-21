#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE 2048

int main(int argc,char *argv[])
{
    int port,fd,i,send,recv,close2,write2,read2;
    struct sockaddr_in addr;
    struct hostent *host;
    char *name;
    char buf1[BUF_SIZE],buf2[BUF_SIZE];
    socklen_t addrsize;

    if(argc != 3) {
        printf("error\n");
        exit(1);
    }

    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(fd<0){
        perror("socket");
        exit(1);
    }

    name = argv[1];
    host = gethostbyname(name);

    port = atoi(argv[2]);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    bcopy(host->h_addr_list[0],(char *)&addr.sin_addr,host->h_length);

    write2=write(1,"send    : ",10);
    if(write2==-1){
      perror("write");
      close(fd);
      exit(1);
    }

    for(i = 0;i < BUF_SIZE;i++) buf1[i] = '\0';
    for(i = 0;i < BUF_SIZE;i++) buf2[i] = '\0';

    read2=read(0,buf1,BUF_SIZE);
    if(read2==-1){
      perror("read");
      close(fd);
      exit(1);
    }

    while(buf1[0]!='\0') {

        addrsize=sizeof(addr);

        send=sendto(fd, buf1,strlen(buf1), 0, (struct sockaddr *)&addr, addrsize);
        if(send<0){
            perror("sendto");
            close(fd);
            exit(1);
        }

        recv=recvfrom(fd,buf2,BUF_SIZE,0,(struct sockaddr *)&addr,&addrsize);
        if(recv<0){
            perror("recvfrom");
            close(fd);
            exit(1);
        }

        printf("receive : %s\n",buf2);
        write2=write(1,"send    : ",10);
        if(write2==-1){
          perror("write");
          close(fd);
          exit(1);
        }

        for(i = 0;i < 100;i++) buf1[i] = '\0';
        for(i = 0;i < 100;i++) buf2[i] = '\0';

        read2=read(0,buf1,BUF_SIZE);
        if(read2==-1){
          perror("read");
          close(fd);
          exit(1);
        }
    }
  
    printf("EOF\n");

    if ((close2=close(fd))==-1){
      perror("close");
      exit(1);
    }

    return 0;
}