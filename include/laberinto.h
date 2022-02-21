#ifndef LABERINTO_H
#define LABERINTO_H
/**
 *  RESUELVE UN LABERINTO.
 *  Raúl Antonio Ortega Vallejo.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/** Patron de Movimientos posibles del tablero */
enum Tablero_Movimientos{DERECHA, IZQUIERDA, 
                         ARRIBA, ABAJO,
		         FIN_MOVIMIENTOS};

/** Configuración del Tablero */
typedef struct Config{
	int filas;
	int columnas;
	int size;
	int inicial;
	int final;
}Config;

/** Nodo Tablero               */
typedef struct Tablero{ 
	char *posicion;           /** Vector char en lugar de matriz int */
	int agente;               /** Posición de la ficha               */
	struct Config *config;    /** Config.                            */
	struct Tablero *sig;      /** Nodo hijo                          */
	struct Tablero *prev;     /** Nodo padre                         */
}Tablero;

typedef struct Lista{
	Tablero *raiz;
	Tablero *actual;
	char *visitado;
	char movimientos[FIN_MOVIMIENTOS];
	int estados;
}Lista;

/** Crear una nueva Config * */
Config * nuevaConfig(void);
/*** Valida una Config creada, error EXIT_FAILURE **/
void validaConfig(Config *creada);
/** Crear un tablero **/
/** Tablero a partir de un nodo padre o NULL;
*/
Tablero *nuevoTablero (Tablero *padre /*  NULL */);
/** Tablero a partir de un FILE *             */
Tablero *getInput(FILE *abierto);
/**  Imprimir Tablero **/
void muestraTablero(Tablero *creado);
void muestraInfoTablero(Tablero *creado); 
/**  Intercambia posición del agente, solo para imprimir */
void swapPosicion(Tablero *);
/**  Operaciones de Lista */
Lista *crearLista(Tablero *creado);
void destruirLista(Lista **creada);
/**
 * Configura en 0 hasta FIN_MOVIMIENTOS en *movimientos
 * devuelve el *movimientos;
 */
char *liberaCamino(char *movimientos);
/**
 * Regresa rand() % totalMovimientos 
 */
int ciego(int totalMovimientos);
/**
 * Regresa el indice de un mov,
 * Error regresa -1
 */
int existeMov(Tablero *actual, int mov);
/**  Booleanas  **/
int visitado(Lista *lista,int mov);
int estadoMeta(Tablero *tablero);
int moverTablero(Lista **lista, int mov);
int atorado(char *recorrido);
/**
 *  Expande la lista (árbol)
 *  Sigue un patron de camino (DERECHA,IZQUIERDA,ARRIBA,ABAJO)
 *    si pudede avanzar continua usando dicho camino.
 */
void expandeLista(Lista **creada);
/**
 *  Expande la lista (árbol)
 *  Simula el comportamiento desconocido, 
 *  Toma paso aleatorio (eje.ARRIBA)
 *    si pudede avanzar continua usando dicho paso.
 */
void expandeRecu(Lista **creada);
/**
 *   Resuelve Laberinto
 *   metodo puede ser 'r' recursivo, 'i' iterativo.
*/
void laberinto(char *laberinto_txt,char metodo);

/** Limpiar Pantalla en Unix                    **/
#define limpiarStream() printf("\e[1;1H\e[2J")

#endif
