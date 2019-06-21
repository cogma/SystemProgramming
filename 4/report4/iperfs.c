#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#define BUF_SIZE 5000000
#define N 100
 
int main(int argc , char *argv[])
{
    int fd,fd2,i,n,port = atoi(argv[1]);
    struct sockaddr_in addr;
    socklen_t addrsize;
    char buf[BUF_SIZE];
    char buf1[50];
    int sent = 0;

    if(argc != 2) {
        printf("error\n");
        exit(1);
    }

    if( (fd = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP)) < 0){
        perror("socket");
        exit(1);
    }
  
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr))<0){
        perror("bind");
        close(fd);
        exit(1);
    }
     
    if (listen(fd, 5) < 0){
        perror("listen");
        close(fd);
        exit(1);
    }
      
    addrsize = sizeof(addr);
    for(i = 0;i < BUF_SIZE;i++) buf[i] = 'a';
        if ((fd2 = accept(fd, (struct sockaddr *)&addr, &addrsize))<0){
            perror("accept");
            close(fd);
            exit(1);
        }
    for(n=0;n<N;n++){
        if((sent = send(fd2,buf,BUF_SIZE,0))<0){
            perror("send");
            exit(1);
        }
    }

    for(i = 0;i < 50;i++) buf1[i] = '\0';
                
    if ((n = recv( fd2 , buf1, 50, 0)) < 0){
        perror("recv");
        exit(1);
    }
    printf("%s\n",buf1);
    close(fd2);
    close(fd); 
    
    return 0;
}
