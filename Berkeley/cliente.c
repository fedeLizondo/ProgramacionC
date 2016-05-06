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

#define IP "127.0.0.1"
#define PUERTO 6668

int main(int argc,char * argv[])
{
	int sd;
	
	struct sockaddr_in pin;
	
	pin.sin_family = AF_INET;
        pin.sin_addr.s_addr = inet_addr(IP);
	pin.sin_port = htons(PUERTO);
	bzero(&pin.sin_zero,sizeof(pin.sin_zero));
	
	sd = socket(AF_INET,SOCK_STREAM,0);
	if( sd < 0 )	
	{
		printf("ERROR al abrir al socket.\n");
		exit(1);
	}
	
	if( connect(sd, (void*)&pin,sizeof(pin)) < 0)
	{
		printf("ERROR al conectar el socket.\n");
		exit(1);
	}
	
	char buffer[100];
	int endBuffer = 0;
	int EXIT = 1000;
	tiempo_t tiempoCliente;
	dameTime(&tiempoCliente);
	printf("Client "); imprimeHora(tiempoCliente.tMaquina);
	
	while(EXIT--)
	{
		snprintf(buffer,100,"%f",tiempoCliente.t);
		write(sd,buffer,100);	
		endBuffer = read(sd,buffer,sizeof(time_t));
		
		if(endBuffer >= 0)
		{
			buffer[endBuffer] = '\0';
			printf("Recibido Tiempo del servidor.\n");
			double tiempoDiferenciaServidor = strtod( buffer, NULL);
			tiempoCliente.diff =  tiempoDiferenciaServidor;
		}

		actualizarSegundo(&tiempoCliente);
		imprimeHora(tiempoCliente.tMaquina);
		sleep(1000);
	}	
}

