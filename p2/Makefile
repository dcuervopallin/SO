all: p2

list.o: list.c list.h
	gcc -c list.c -Wall -g
	
p2.o: p2.c list.h comandos_p0.c comandos_p1.c comandos_p2.c ayudaP1.c ayudaP2.c process_shell.c
	gcc -c p2.c comandos_p0.c comandos_p1.c comandos_p2.c ayudaP1.c ayudaP2.c process_shell.c -Wall -g
	
p2: p2.o list.o
	gcc -o p2 p2.o list.o -Wall -g

clean:
	rm -f p2 comandos_p0.o comandos_p1.o comandos_p2.o ayudaP1.o ayudaP2.o p2.o list.o process_shell.o
