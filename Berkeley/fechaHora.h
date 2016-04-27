#ifndef FECHAHORA_H
#define FECHAHORA_H
#include <stdio.h>
#include <time.h>

void DameFecha( char * fecha )
{
	time_t tiempo = time(0);
	struct tm * tlocal = localtime(&tiempo);
	printf("El resultado es %d.\n",strftime(fecha,128,"%d/%m/%y",tlocal));
}

void DameHora(char * hora)
{
	time_t tiempo = time(0);
	struct tm * tlocal = localtime(&tiempo);
	strftime(hora,128,"%H:%M:%S",tlocal);
}


void DameSoloHora(char * hora)
{
	time_t tiempo = time(0);
	struct tm *tlocal = localtime(&tiempo);
	strftime(hora,128,"%H",tlocal);
}

#endif /*FIN FECHAHORA_H */
