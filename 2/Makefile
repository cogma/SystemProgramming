CC = gcc
LD = gcc
CFLAGS = -Wall
LDFLAGS =

all: mycp mywc

mycp: mycp.o
	$(LD) $(LDFLAGS) –o $@ $^

mywc: mywc.o
	$(LD) $(LDFLAGS) –o $@ $^

clean:
	rm -f *.o mycp mywc

.PHONY: all clean
