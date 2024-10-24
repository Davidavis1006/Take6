all: take6.o main.c
	gcc main.c take6.o -o main

take6.o: take6.c
	gcc -c take6.c -o take6.o

clean:
	rm -f main *.o