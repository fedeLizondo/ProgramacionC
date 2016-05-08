#include <arpa/inet.h>
#include <fcntl.h> //Borrar
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "fecha.h"

#define IP "127.0.0.1"//"192.168.1.103"
#define PUERTO 6666

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


int main(int argc,char * argv[])
{
	int sd;
        struct hostent *server;	
	struct sockaddr_in pin;
	
	pin.sin_family = AF_INET;
        pin.sin_addr.s_addr = inet_addr(IP);
	pin.sin_port = htons(PUERTO);
	memset(pin.sin_zero,0,8);	

	sd = socket(AF_INET,SOCK_STREAM,0);
	if( sd < 0 )	
	{
		printf(ANSI_COLOR_RED"ERROR al abrir al socket.\n"ANSI_COLOR_RESET);
		exit(1);
	}
	printf(" Socket es  %d.\n",sd);
	
	if( connect(sd, (struct sockaddr *)&pin,sizeof(struct sockaddr_in)) ==-1)
	{
		printf(ANSI_COLOR_RED"ERROR al conectar el socket.\n"ANSI_COLOR_RESET);
		exit(1);
	}
	
	char buffer[100];
	int endBuffer = 0;
	int EXIT = 1000;
//	tiempo_t * tClient = malloc(sizeof(tiempo_t));
	tiempo_t tiempoCliente;
	dameTime(&tiempoCliente);
//	tiempoCliente = *tClient;
	
	printf("Cliente "); imprimeHora(tiempoCliente.tMaquina);
		
	while(EXIT--)
	{
		//printf("%d Envio ----> %lf \n",EXIT , (double)tiempoCliente.t);
		
		//CONVIERTE DE STRING A DOUBLE TIEMPO CLIENTE 
		snprintf(buffer,100,"%lf",(double)tiempoCliente.t);
		
		//ENVIO AL SERVIDOR TIEMPO DEL CLIENTE EN FLOAT
		write(sd,buffer,100);	

		//RECIBO LA RESPUESTA DEL SERVIDOR
		endBuffer = read(sd,buffer,100);
		
		if( endBuffer >= 0)
		{
			buffer[endBuffer] = '\0';
			
			double tiempoDiferenciaServidor = strtod( buffer, NULL);
			if( tiempoDiferenciaServidor > 1)
			{
				
				printf("Sincronizando servidor \nANTES "ANSI_COLOR_RED );
				imprimeHora(tiempoCliente.tMaquina);
				printf(ANSI_COLOR_RESET);
				printf("%.lf s diferencia con el servidor.\n",tiempoDiferenciaServidor);
				
				tiempoCliente.diff =  tiempoDiferenciaServidor;
				actualizarSegundo(&tiempoCliente);
				
				printf("DESPUES "ANSI_COLOR_GREEN); 
				imprimeHora(tiempoCliente.tMaquina);
				printf(ANSI_COLOR_RESET);
			}
		}
		
		sleep(1);
	}
	//CIERRA LA CONEXIÓN DESPUES DE UN 
	char b[]="exit";	
	write( sd , b , (sizeof(char)*5) );

}
