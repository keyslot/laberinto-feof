CC=gcc
SRC=src/
MAIN=main
FLAGS=-Wall -Wextra
CLASS=laberinto
INCLUDE=include

.PHONY:	main clean

main:
	$(CC) -o $(MAIN) $(MAIN).c -I$(INCLUDE) $(SRC)$(CLASS).c $(FLAGS) 

clean:
	rm  -f $(MAIN) *.o 

