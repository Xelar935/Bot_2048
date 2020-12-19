all:
	chmod u=rwx Bot_2048.c
	gcc Bot_2048.c -o 2048 -lm
	./2048
