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
#include <pthread.h>

#define BUF_SIZE 2048
#define NTHREAD 2

struct sockaddr_in addr,addr2;
pthread_t threads[NTHREAD];

void end(int x){
    printf("  --- end ---\n");
    exit(0);
}

void *server (void *arg) {
    int s,i,close2,recv2,send2,ss=*(int *)arg;
    char buf1[BUF_SIZE];
    socklen_t addrsize;

    while(1){
        addrsize = sizeof(addr);
        s = accept(ss,(struct sockaddr *)&addr,&addrsize);
        if(s<0){
            perror("accept");
            close(ss);
            exit(1);
        }
        while(1){
            for(i = 0;i < BUF_SIZE;i++) buf1[i] = '\0';

            recv2=recv(s,buf1,BUF_SIZE,0);
            if(recv2<0){
                perror("recv");
                close(s); 
                exit(1);
            }
            if(buf1[0]=='\0'){
                break;
            }
            printf("receive : %s",buf1);

            send2=send(s,buf1,recv2,0);
            if(send2<0){
                perror("send");
                close(s);
                exit(1);
            }
            printf("send    : %s\n",buf1);
        }
        close2=close(s);
        if (close2==-1){
            perror("close");
            exit(1);
        }
    }
    pthread_exit(NULL);
}

int main (int argc,char *argv[]) {
    int s,i,binded,listen2,created,joined,port,close2;

    if (signal(SIGINT,end) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    if(argc != 2) {
        printf("error\n");
        exit(1);
    }

    port = atoi(argv[1]);

    addr2.sin_family = AF_INET;
    addr2.sin_port = htons(port);
    addr2.sin_addr.s_addr = INADDR_ANY;

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(s<0){
        perror("socket");
        exit(1);
    }

    binded=bind(s,(struct sockaddr *)&addr2,sizeof(addr2));
    if(binded<0){
        perror("bind");
        close(s);
        exit(1);
    }

    listen2=listen(s,5); 
    if(listen2<0){
        perror("listen");
        close(s);
        exit(1);
    }

    for ( i = 0; i < 2; ++i ){
        created=pthread_create(&threads[i],NULL,server,&s);
        if(created!=0){
            printf("error: pthread_create\n");
            exit(1);
        }
    }

    for ( i = 0; i < 2; ++i ){
        joined=pthread_join(threads[i],NULL);
        if(joined!=0){
            printf("error: pthread_join\n");
            exit(1);
       }
    }

    if ((close2=close(s))==-1){
        perror("close");
        exit(1);
    }
    return 0;
}