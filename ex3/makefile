.PHONY: all clean

FLAGS = -Wall -Wextra -Wvla

all: sudukusolver genericdfs.a

sudukusolver: sudukusolver.o genericdfs.o sudukutree.o
	gcc -g $(FLAGS) sudukusolver.o genericdfs.o sudukutree.o -o  sudukusolver -lm

sudukusolver.o: sudukusolver.c genericdfs.h sudukutree.h
	gcc -g -c $(FLAGS) sudukusolver.c
	
genericdfs.o: genericdfs.c genericdfs.h
	gcc -g -c $(FLAGS) genericdfs.c

genericdfs.a: genericdfs.o
	ar rcs genericdfs.a genericdfs.o
	
sudukutree.o: sudukutree.c sudukutree.h
	gcc -g -c $(FLAGS) sudukutree.c

clean:
	rm -f *.o sudukusolver genericdfs.a
