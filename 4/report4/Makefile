udpechoserver: udpechoserver.c
	gcc -Wall udpechoserver.c -o udpechoserver
tcpechoserver1: tcpechoserver1.c
	gcc -Wall -pthread tcpechoserver1.c -o tcpechoserver1

tcpechoserver2: tcpechoserver2.c
	gcc -Wall tcpechoserver2.c -o tcpechoserver2

udpechoclient: udpechoclient.c
	gcc -Wall udpechoclient.c -o udpechoclient

tcpechoclient1: tcpechoclient1.c
	gcc -Wall tcpechoclient1.c -o tcpechoclient1

iperfs: iperfs.c
	gcc -Wall iperfs.c -o iperfs

iperfc: iperfc.c
	gcc -Wall iperfc.c -o iperfc

clean:
	rm -f udpechoserver udpechoclient tcpechoserver1 tcpechoserver2 tcpechoclient1 iperfs iperfc
