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
#include "utils.h"

#define IP "127.0.0.1"//"192.168.1.103"
#define PUERTO 6666


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
	printf(" Socket es "ANSI_BOLD_START"%d"ANSI_BOLD_END".\n",sd);
	
	if( connect(sd, (struct sockaddr *)&pin,sizeof(struct sockaddr_in)) ==-1)
	{
		printf(ANSI_COLOR_RED"ERROR al conectar el socket.\n"ANSI_COLOR_RESET);
		exit(1);
	}
	
	char buffer[100];
	int endBuffer = 0;
	int EXIT = 1000;
	tiempo_t tiempoCliente;
	dameTime(&tiempoCliente);
	printf(ANSI_BOLD_START"Cliente "); imprimeHora(tiempoCliente.tMaquina);
	printf(ANSI_BOLD_END);	
	while(EXIT--)
	{
		
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
			if( tiempoDiferenciaServidor != 0  )
			{
				
				printf("Sincronizando servidor \nANTES "ANSI_COLOR_RED );
				imprimeHora(tiempoCliente.tMaquina);
				printf(ANSI_COLOR_RESET);
				printf("%.lf s diferencia con el servidor.\n",tiempoDiferenciaServidor);
				
				tiempoCliente.diff =  tiempoDiferenciaServidor - 1 ;
				actualizarSegundo(&tiempoCliente);
				
				printf("DESPUES "ANSI_COLOR_GREEN); 
				imprimeHora(tiempoCliente.tMaquina);
				printf(ANSI_COLOR_RESET);
			//	printf("\033[4A"); //Vuelve el cursor 4 lugares arriba
			}
	//	actualizarSegundo(&tiempoCliente); //TODO BORRAR COMENTARIO PARA SINCRONIZAR

		}
		imprimeHora(tiempoCliente.tMaquina);		

	//	printf("\033[1A");//Vuelve el cursor 1 lugar arriba 
		
		sleep(1);
	}
	//CIERRA LA CONEXIÓN DESPUES DE UN TIEMPO
	char b[]="exit";	
	write( sd , b , (sizeof(char)*5) );
		
	close(sd);
	return EXIT_SUCCESS;	
}
