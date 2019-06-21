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
#include <sys/time.h>

#define BUF_SIZE 5000000
#define N 100

int main(int argc,char *argv[])
{
    int port,fd,i,send2,recv2,connect2,close2;
    struct sockaddr_in addr;
    struct hostent *host;
    char *name;
    char buf[BUF_SIZE],buf1[30]={'\0'};
    struct timeval t0;
    struct timeval t1;
    long int t,tt;
    long int sum = 0;

    if(argc != 3) {
        printf("error\n");
        exit(1);
    }

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

    connect2=connect(fd,(struct sockaddr *)&addr,sizeof(addr));
    if(connect2<0){
        perror("connect");
        close(fd);
        exit(1);
    }

    for(i = 0;i < BUF_SIZE;i++) buf[i] = '\0';

    gettimeofday(&t0, NULL);

    while(sum < N * BUF_SIZE){
        recv2=recv(fd,buf,BUF_SIZE,0);
        if(recv2<0){
            perror("recv");
            exit(1);
        }
        sum = sum + recv2;
    }

    gettimeofday(&t1, NULL);

    if (t1.tv_usec < t0.tv_usec) {
    	t=t1.tv_sec - t0.tv_sec - 1;
        tt=1000000 + t1.tv_usec - t0.tv_usec;
	}
    else {
	    t=t1.tv_sec - t0.tv_sec;
        tt=t1.tv_usec - t0.tv_usec;
    }
        
    sprintf(buf1,"%ld %ld.%06ld %f",(long int)sum,t,tt,8.0*sum/(t+tt/1000000.0)/1000000.0);

    send2=send(fd, buf1,strlen(buf1),0);
    if(send2<0){
        perror("send");
        exit(1);
    }

    printf("%s\n",buf1);

    if ((close2=close(fd))==-1){
      perror("close");
      exit(1);
    }

    return 0;
}
