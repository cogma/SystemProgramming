#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>

int main()
{
	struct timeval t0;
	struct timeval t1;
	struct timeval t2;
	int i=0;

	gettimeofday(&t0, NULL);
	getpid();
	gettimeofday(&t1, NULL);
	for(i=0;i<1000;i++){
	  getpid();
	}
	gettimeofday(&t2, NULL);

	if (t1.tv_usec < t0.tv_usec) {
	  printf("%ld.%06ld s\n", t1.tv_sec - t0.tv_sec - 1, 1000000 + t1.tv_usec - t0.tv_usec);
	}
	else {
	  printf("%ld.%06ld s\n", t1.tv_sec - t0.tv_sec, t1.tv_usec - t0.tv_usec);
	}

	if (t2.tv_usec < t1.tv_usec) {
	  printf("%ld.%06ld s\n", t2.tv_sec - t1.tv_sec - 1, 1000000 + t2.tv_usec - t1.tv_usec);
	}
	else {
	  printf("%ld.%06ld s\n", t2.tv_sec - t1.tv_sec, t2.tv_usec - t1.tv_usec);
	}
	return 0;
}
