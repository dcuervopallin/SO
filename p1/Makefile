all: p1

list.o: list.c list.h
	gcc -c list.c -Wall -g
	
p1.o: p1.c list.h comandos_p0.c comandos_p1.c ayudaP1.c process_shell.c
	gcc -c p1.c comandos_p0.c comandos_p1.c ayudaP1.c  process_shell.c -Wall -g
	
p1: p1.o list.o
	gcc -o p1 p1.o list.o -Wall -g

clean:
	rm -f p1 comandos_p0.o comandos_p1.o ayudaP1.o p1.o list.o process_shell.o
