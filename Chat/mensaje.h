#include <stdio.h>
#include <stdlib.h>

#define DISPONIBLE 0
#define OCUPADO 1

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct mensaje{
	char msn[80];
	short estado;
	int destinatario; //ID DE QUIEN LO ENVIA
	int destino;      //ID DE QUIEN RECIVE
	char apodoDestinatario[20];
	char apodoDestino[20];
}mensaje;

void imprimirMensaje(mensaje * m)
{
	printf( "De:"ANSI_COLOR_RED"%s"ANSI_COLOR_RESET" Para:"ANSI_COLOR_RED" %s"ANSI_COLOR_RESET"\n",(*m).apodoDestinatario,(*m).apodoDestino ); 
	printf( ANSI_COLOR_YELLOW"MENSAJE:"ANSI_COLOR_RESET"\n%s\n\n",(*m).msn);
}
void mostrarDatos(mensaje * m)
{
	printf("%s \n %s \n %s \n %s \n %s \n %s \n",m->msn,m->estado,m->destinatario,m->destino,m->apodoDestinatario,m->apodoDestino);
}


