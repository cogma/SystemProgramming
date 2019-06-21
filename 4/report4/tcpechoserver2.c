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
#include <signal.h>

#define BUF_SIZE 2048
#define CLIENTS 2

void end(int x){
    printf("  --- end ---\n");
    exit(0);
}
 
int main(int argc , char *argv[])
{
    int fd,fd2,clients[CLIENTS],selected,i,n,sd,port = atoi(argv[1]);
    int max;
    struct sockaddr_in addr;
    socklen_t addrsize;
    char buf1[BUF_SIZE];
    fd_set fdset;

    if (signal(SIGINT,end) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    if(argc != 2) {
        printf("error\n");
        exit(1);
    }

    for (i = 0; i < CLIENTS; i++){
        clients[i] = 0;
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
     
    while(1){
        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
        max = fd;
         
        for ( i = 0 ; i < CLIENTS ; i++){
            sd = clients[i];
            if(sd > 0) FD_SET( sd , &fdset);
             
            if(sd > max) max = sd;
        }
  
        selected = select( max + 1 , &fdset , NULL , NULL , NULL);
    
        if (selected < 0){
            printf("select");
            close(fd);
            exit(1);
        }
          
        if (FD_ISSET(fd, &fdset)){
            if ((fd2 = accept(fd, (struct sockaddr *)&addr, &addrsize))<0){
                perror("accept");
                close(fd);
                exit(1);
            }
          
            for (i = 0; i < CLIENTS; i++){
                if( clients[i] == 0 ){
                    clients[i] = fd2; 
                    break;
                }
            }
        }
          
        for (i = 0; i < CLIENTS; i++){
            sd = clients[i];
              
            if (FD_ISSET( sd , &fdset)){
                for(i = 0;i < BUF_SIZE;i++) buf1[i] = '\0';
                if ((n = recv( sd , buf1, BUF_SIZE,0)) < 0){
                    perror("recv");
                    close( sd );
                    exit(1);
                }
                if(buf1[0]=='\0'){
                    clients[i] = 0;
                    break;
                }
                printf("receive : %s",buf1);
                if(send(sd,buf1,n,0)<0){
                    perror("send");
                    close(sd);
                    exit(1);
                }
                printf("send    : %s\n",buf1);
            }
        }
    }
      
    return 0;
}