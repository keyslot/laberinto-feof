#include "laberinto.h"

int main(int argc, char **argv) {
       if (argc < 3 || (argv[1][1] != 'r' && argv[1][1] != 'i')) {
           printf("Uso: %s -[opcion] laberintos/laberinto.txt\n", argv[0]);
	   printf("\t-r Recursivo: Pasos aleatorios.\n");
           printf("\t-i Iterativo: Siguiendo patrón.\n");
           return -1;
	}
	laberinto(argv[2],argv[1][1]);
	return 0;
}


