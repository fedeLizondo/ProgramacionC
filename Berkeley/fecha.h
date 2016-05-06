#ifndef FECHA_H
#define FECHA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tiempo_t{
	time_t t;
	struct tm * tMaquina;
	double diff;
}tiempo_t;


void inicializarTiempo( tiempo_t * t )
{
	if( t != NULL )
	{
		t->t = 0 ;
		t->tMaquina = NULL;
		t->diff = 0;
	}
	else
		printf("ERROR: t is NULL");
}


/*
 * Esta funcion Muestra el tiempo local
 * @param = ninguno;
 */
void dameTime( tiempo_t * t )
{	
	if( t == NULL )
	{
		t = (tiempo_t *) malloc(sizeof(tiempo_t));
		t->t = 0;
		t->tMaquina = NULL;
		t->diff = 0;
	}

	if( t != NULL)
	{
		t->t = time(0);
		if(t->tMaquina != NULL)
			free(t->tMaquina);
		t->tMaquina = localtime( &t->t);
		t->diff = 0;
	}
	else
		printf("Error: Out of memory");
}

/*
 * Devuelve la difrencia entre 2 horas
 * @param local es el tiempo de la maquina local
 * @param externo es el recidido de otra maquina
 */
double differenciaSegundos(struct tm * local, struct tm * externo)
{
	int aux = 0;
	if(local != NULL && externo != NULL)
	{
		time_t t1 = mktime(local),
		       t2 = mktime(externo);
		aux = difftime(t1,t2);
	}
	return	aux;
}

/*
 * Esta funcion va imprimir el tiempo que se pasa por argumento
 * El formato va a ser dd/mm/aa ss:mm:hh 
 * @param t = tiempo a imprimir 
 */
void imprimeHora(const struct tm * t)
{
	if( t == NULL)
		printf("Error : struct tm is NULL");
	else
	{
		char texto[28];
		strftime(texto,28,"Date: %d/%m/%Y  %H:%M:%S\n",t);
		printf(texto);
	}
}

/*
 * Esta funcion actualizara un segundo el tiempo y si se tiene un offset
 * se acutalizara
 * @param t = tiempo a actualizar
 */
void actualizarSegundo( tiempo_t * t )
{
	if( t != NULL )
	{
		t->t = t->t + t->diff + 1;
		free( t->tMaquina );
		t->tMaquina = localtime( &t->t );		
		t->diff = 0;	
	}
	else
		printf("ERROR : t is NULL");
}

time_t * getStructTime( tiempo_t * t)
{
	if(t != NULL)
		return &t->t;
	else
		printf("ERROR : t is NULL");	
	return 0;
}


#endif /* FIN FECHA_H */
