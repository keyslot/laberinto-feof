#include "laberinto.h"

static char *describe[FIN_MOVIMIENTOS] = {
	                           "DERECHA",
                                   "IZQUIERDA",
                                   "ARRIBA",
			           "ABAJO"
			            };

static void
exitIF(int expr) {
       if(expr) {
          perror("ERROR:");
	  exit(EXIT_FAILURE);
        }
}

void 
muestraTablero(Tablero *tablero) 
{
	Config *config = tablero->config;
	int i,j;
	printf("Muestra tablero...\n");
	for(i = 0; i < config->filas; i++) {
	    for(j = 0; j < config->columnas; j++)
	        putchar(tablero->posicion[i * config->columnas + j]);
	    putchar('\n');
	}
	limpiarStream();
	sleep(1);
}

void
muestraInfoTablero(Tablero *tablero) 
{
	muestraTablero(tablero);
	printf("\nfilas:%d",tablero->config->filas);
	printf("|columnas:%d\n",tablero->config->columnas);
	limpiarStream();
}

void 
swapPosicion(Tablero *tablero)
{
	int old = tablero->config->inicial;
	int swap = tablero->agente;
	char c = tablero->posicion[old];
	tablero->posicion[old] = ' ';
	tablero->posicion[swap] = c;
	muestraTablero(tablero);
	tablero->posicion[swap] = ' ';
	tablero->posicion[old] = c;
}

Tablero *
nuevoTablero (Tablero *padre)
{
	Tablero *nuevo = malloc(sizeof(Tablero));
	exitIF(!nuevo);
	nuevo->sig = NULL;
	nuevo->prev = NULL;
	if(padre) {
		memcpy(nuevo, padre,sizeof(Tablero));
		return nuevo;
	}
	nuevo->posicion = NULL;
	nuevo->config = NULL;
	return nuevo; 
}

Config *
nuevaConfig(void)
{
	Config *config = malloc(sizeof(Config));
	exitIF(!config);
	config->columnas = 0;
	config->filas    = 0;
	config->size     = 0;
	config->inicial  = 0;
	config->final    = 0;
	return config;
}

void 
validaConfig(Config *config)
{
	if(config->size < 2  || config->size % config->columnas) {
	   puts("Laberinto mal formado...");
	   exit(EXIT_FAILURE);
	}
}

Tablero *
getInput(FILE *input)
{
	Tablero *tablero =  nuevoTablero(NULL);
	tablero->config = nuevaConfig();
	char *vector = malloc(sizeof(char));
	exitIF(!vector);
	int c = '\0';
	int i = 0;
	while( !feof(input) ){
		c = getc(input);
		if( c != '\n' ) {
		    vector[i] = c;
		    if(vector[i] == 'I') 
		       tablero->config->inicial = i;
		    else if(vector[i] == 'F')
		       tablero->config->final = i;
	  	    vector = realloc(vector,i + 2);
		    exitIF(!vector);
		    i++;
		}
	       else if(!tablero->config->columnas)
		        tablero->config->columnas = i;
	}
	i--; /* EOF */
	/* Config . tablero ...                      */
	tablero->posicion = vector;
	tablero->config->filas = i / tablero->config->columnas;
	tablero->config->size = i;
	tablero->agente = tablero->config->inicial;
	validaConfig(tablero->config);
	return tablero;
}

Lista *
crearLista(Tablero *tablero)
{
	int dimensiones = tablero->config->size;
	Lista *lista = malloc(sizeof(Lista));
	exitIF(!lista);
	lista->raiz = tablero;
	lista->actual = tablero;
	lista->visitado = malloc(sizeof(char) * dimensiones + 1);
	exitIF(!lista->visitado);
	memset(lista->visitado,0,dimensiones);
	lista->estados = 0;
	return lista;
}

void 
destruirLista(Lista **lista)
{
	if(!lista || !*lista) return;
	Tablero *ptr = (*lista)->raiz;
	Tablero *aux = NULL;
	free(ptr->config);
	free(ptr->posicion);
	while(ptr) {
		aux = ptr->sig;
		free(ptr);
		ptr = aux; 
	}
       aux = ptr = NULL;
       free((*lista)->visitado);
       free(*lista);
       *lista = NULL;
}

int 
visitado(Lista *lista, int mov)
{
	return lista->visitado[mov];
}

int 
ciego(int limit)
{
	time_t t;
	usleep(500);
	srand((unsigned) time(&t));
	return rand() % limit;
}

int 
existeMov(Tablero *actual, int mov) 
{
	int intento = actual->agente + 1;
	int columna = actual->config->columnas;
	int size = actual->config->size;
	switch(mov) {
		case ARRIBA:
                     intento -= columna;
		     break;
		case ABAJO: 
		     intento += columna;
		     break;
		case DERECHA: 
		     if(intento % columna == 0) return -1;
		     intento++;
		     break;
		case IZQUIERDA:
		     intento--;
		     if(intento % columna == 0) return -1;
		     break;
		default:
		     return -1;
		     break;
	}
       if((intento > 0 && intento <= size) && 
	       actual->posicion[intento-1] != 'x')
	       return intento-1;
       else 
               return -1;
}

int 
estadoMeta(Tablero *tablero)
{
	return (tablero->agente == tablero->config->final);
}

int
moverTablero(Lista **lista, int mov) 
{	
	Tablero *tab = (*lista)->actual;
	int explorado = 0;
	int movimiento = mov;
        if(!estadoMeta(tab)) {
           mov = existeMov(tab,mov);
           if(mov < 0 ) 
	      return 0;
	   explorado = visitado(*lista,mov);
	   if(!explorado) {
	      printf("\r Intentando ... \t   %s     ",describe[movimiento]);
	      Tablero *nuevo = nuevoTablero(tab);
	      nuevo->agente =  mov;
	      tab->sig = nuevo;
	      nuevo->prev = tab;
	      (*lista)->actual = nuevo;
	      swapPosicion(nuevo);
	      (*lista)->visitado[mov] = 1;
	      (*lista)->estados++;
              return 1;
	     }
	}  
        return 0;
 }

char *
liberaCamino(char *camino)
{
	camino[0] = 0;
	camino[1] = 0;
	camino[2] = 0;
	camino[3] = 0;
	return camino;
}

int 
atorado(char *recorrido)
{
	int i = 0;
	for (; i < FIN_MOVIMIENTOS; i++)
		if(recorrido[i] == 0) 
			return 0;
	printf("----------------------- atorado!!!!\n");
	return 1;
}

void
expandeLista(Lista **lista)
{
	while(!estadoMeta((*lista)->actual)) {
		/* Este es un patron de movimientos */
		if (!moverTablero(lista, IZQUIERDA) &&
		    !moverTablero(lista, DERECHA) &&
		    !moverTablero(lista, ARRIBA) &&
		    !moverTablero(lista, ABAJO)) {
		
		  if((*lista)->actual->prev) {
		    (*lista)->actual = (*lista)->actual->prev;
	            printf("\r Estado visitado     "); 
		    swapPosicion((*lista)->actual);
	            }
		  else {
			break;
		     } 
	         }
	}
	
}

void
expandeRecu(Lista **lista)
{	
        int mov = 0;
	int prev = -1;
	char *camino = liberaCamino((*lista)->movimientos);
	while(!estadoMeta((*lista)->actual)) {
               mov = ciego(FIN_MOVIMIENTOS);
	       if (prev == mov) 
		   continue;
 	       while(moverTablero(lista,mov)) {
		       camino = liberaCamino(camino);
	       }
	      prev = mov;
	      if(atorado(camino)) 
		  break;
	      camino[mov] = 1;
	   }
	 if(!estadoMeta((*lista)->actual) && (*lista)->actual->prev) {
            (*lista)->actual = (*lista)->actual->prev;
            swapPosicion((*lista)->actual);
            expandeRecu(lista); 
           }	  	
}

void
laberinto(char *file, char form)
{
	FILE *f = fopen(file,"r");
	if(!f) 
	   return perror(file);
	Lista *nueva = crearLista(getInput(f));
	muestraInfoTablero(nueva->actual);
	switch(form){
		case 'r':
		        puts("RECURSIVO");
		        expandeRecu(&nueva);
		break;
		case 'i':
		        puts("ITERATIVO");
		        expandeLista(&nueva);
		break;
		default:
		        puts("ITERATIVO");
		        expandeLista(&nueva);
		break;
	    }
	if(!estadoMeta(nueva->actual))
	   printf("......... Estado meta imposible\n");
        printf("\n Estados: %d\n", nueva->estados);
        destruirLista(&nueva);
	fclose(f);
	f = NULL;
}
