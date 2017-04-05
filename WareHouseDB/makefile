CFLAGS = -Wextra -Wall

waredb: waredb.o 
	gcc waredb.o -o waredb

CheckParenthesis: CheckParenthesis.o
	gcc CheckParenthesis.o -o CheckParenthesis


waredb.o: waredb.c
	gcc $(CFLAGS) -c waredb.c

CheckParenthesis.o: CheckParenthesis.c
	gcc $(CFLAGS) -c CheckParenthesis.c



all: waredb CheckParenthesis

clean:
	rm -f *.o waredb CheckParenthesis

.PHONY: all clean
