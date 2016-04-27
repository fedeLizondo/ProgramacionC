#ifndef "FECHA_HORA_H"
#define "FECHA_HORA_H"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tiempo
{
	int anio;
	int mes;
	int dia;

	int hora;
	int minuto;
	int segundo;

}tiempo;

/*
 * Inicializa una estructura t o cargar los datos de una estructura t
 * con el tiempos actuales
 * @param t = struct tiempo
 */
tiempo * getTime( tiempo * t  )
{
	if( t == NULL)
		t = malloc(sizeof(tiempo));
	if(t == NULL);
		printf("ERROR : Out of memory");
	else
	{
		time_t tp = time(0);
		struct tm *tl = localtime(&tp);	
		
		t->dia = tl->tm_mday;
		t->mes = tl->tm_mon;
		t->anio = tl->tm_year;

		t->hora = tl->tm_hour;
		t->minuto = tl->tm_min;
		t->segundo = tl->tm_sec;
	}	

	return t;
}

/*
 * Imprime la hora con un formato hh:mm:ss
 * @param t = struct tiempo
 */
void imprimirHoraCompleta(tiempo * t)
{
	if( t != NULL)
		printf("%d:%d:%d",t->hora,t->minuto,t->segundo;
	else
		printf("Error : Pointer to tiempo is NULL");
}

/*
 * imprime una fecha con el formato dd/mm/aaaa
 * @param t = struct tiempo
 */
void imprimirFecha(tiempo * t)
{
	if( t != NULL )
		printf("%d/%d/%d",t->dia,t->mes,t->anio);
	else
		printf("Error : Pointer to tiempo is NULL");
}

/*
 * devuelve la diferencia entre 2 tiempos hhmmss en segundos
 * @param = t1 struct tiempo
 * @param = t2 struct tiempo
 */ 
int diferencia (tiempo * t1 ,tiempo * t2) 
{
	if(t1 == NULL || t2 == NULL
	{
		printf("ERROR: %s %s NULL .\n",t1==NULL?"t1":"t2",t1==NULL && t2==NULL ? "and t2 are":" is");
	}		
	int t1Seg = 0;
	int t2Seg = 0;

	if(t1 != NULL)       
		t1->segundo + (t1->minuto * 60) + (t1->hora * 360 );

	if(t2 != NULL)
		t2->segundo + (t2->minuto * 60) + (t2->hora * 360 );  

	return t1Seg - t2Seg;
}
/*
 *Esta funcion permite que el tiempo toSet sea igual a setter
 *@param toSet = struc tiempo a ajustar
 *@param setter = struct tiempo es el que lleva el horario a ajustar
 */

tiempo *setTiempo(tiempo * toSet,tiempo * setter)
{

	if(t1 == NULL)
	{
		t1 = malloc(sizeof(tiempo));			
		if(t1 != NULL)
		{
			t1 = getTime(NULL);
							
		}
		else
			printf("ERROR: Out of memory");
	}


	if(t1 != NULL && t2 != NULL)
	{
		t1->dia = t2->dia;
		t1->mes = t2->mes;
		t1->anio = t2->anio;

		t1->hora = t2->hora;
		t1->minuto = t2->minuto;
		t2->segundo = t2->segundo;
			
	}
	else
		printf("ERROR: t1 or/and t2 is NULL");
	
	return t1;
}

/*
 * Esta funcion ajusta el tiempo hh:mm:ss segun los segundos , pudiendo
 * atrazar o adelantar el tiempo
 *@param t = struct tiempo a ajustar
 *@param segundosAjuste = son los segundos a ajustar (+/-)
 */
void ajustarTiempo( tiempo * t , int segundosAjuste )
{
	if(t != NULL)
	{
		int seg = segundosAjuste % 60;
		int min = ((segundosAjuste)/60)%60;
		int hor = ((segundosAjuste/60)/60)%24;	
		
		t->seg += seg;
		if(t->seg >= 60) 
		{
			min++ ;
			t->seg = t->seg % 60; 
		}
		
		t->minuto += min;
		
		if(t->minuto >= 60)
		{
			hor++;
			t->minuto = t->minuto % 60;
		}
		
		t->hora += hora;
		if ( t->hora >=24)
		{
			t->dia++;
			t->hora = t->hora%24;		
		}		
	}		
}	

void update1Seg(tiempo * t)
{
	
	t->seg++;
	if(t->seg >= 60)
	{
		t->seg = 0;
		t->minuto++;
	}
	if(t->minuto >= 60)
	{
		t->minuto = 0;
		t->hora++;
	}
	if(t->hora >= 24 )
	{
		t->dia++;
	}
	//TODO modificar para dia mes anio
}	

#endif /* FIN FECHA_HORA_H */
