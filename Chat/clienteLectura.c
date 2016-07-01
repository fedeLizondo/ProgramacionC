#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "mkaddr.c"
#include "mensaje.h"
#include <pthread.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define ANSI_COLOR_RED	"\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
 int mkaddr( void *addr,
                   int *addrlen,
                   char *str_addr,
                   char *protocol);

char buffer[256];//BUFFER COMPARTIDO
char auxiliar[256];
/*
 * This function reports the error and
 * exits back to the shell:
 */
static void
displayError(const char *on_what) {
     fputs(strerror(errno),stderr);
     fputs(": ",stderr);
     fputs(on_what,stderr);
     fputc('\n',stderr);
     exit(1);
}
int IDCLIENTE ;
char apodo[20];
char apodoServer[20];

int main(int argc,char **argv)
{
	if(argc > 1)
	{
		printf("LECTURA RECIVE\n%s\n",argv[1]);	fflush(stdout);
		IDCLIENTE = atoi(argv[1]);	
	}
	
	IDCLIENTE = 1;
	//////////////////////////////////////////////////////////////
	//		INICIALIZO EL SERVIDOR                      // 	
	//////////////////////////////////////////////////////////////	
		
	//printf("Ingrese EL ID CLIENTE \n");
	//scanf("%d",&IDCLIENTE);
	system("clear");
	
	struct sockaddr_in servidor_sock;
	int idsock_servidor, // codigo identificador de socket del lado de servidor
    	    length_sock = sizeof(struct sockaddr_in);
	    	    	
	idsock_servidor = socket(AF_INET,SOCK_STREAM,0);
	     			
	printf("ID Socket Servidor %d \n",idsock_servidor);
	    
	servidor_sock.sin_family      = AF_INET;
	servidor_sock.sin_port        = htons(6667);
	servidor_sock.sin_addr.s_addr = inet_addr("127.0.0.99");	
	memset(servidor_sock.sin_zero,0,8);	    
	
	if( connect( idsock_servidor, (struct sockaddr *)&servidor_sock,sizeof(struct sockaddr_in)) ==-1)
	{
		printf(ANSI_COLOR_RED"ERROR al conectar el socket.\n"ANSI_COLOR_RESET);
		exit(1);
	}
	
	////////////////////////////////////////////////////////
	//  HANDSHAKE CON SERVIDOR PARA IDENTIFICAR CLIENTE   //
        ////////////////////////////////////////////////////////
	int tamBuffer;
	char msn[80];
	//char apodo[20];
	mensaje m;	
	
	//Envio la ID del Cliente Para Identificarme		
	bzero(buffer,100);	
	sprintf( buffer, "%d", IDCLIENTE );
	write(idsock_servidor,buffer,100);
	
	//RECIVO EL APODO DEL CLIENTE
	bzero(buffer,100);
	tamBuffer = read(idsock_servidor,buffer,20);
	buffer[tamBuffer]='\0';
	printf("%s\n",buffer);

	strcpy(m.apodoDestino,buffer);
	
	for(;;)
	{	
		
		//RECIVO EL MENSAJE
		bzero(buffer,100);
		tamBuffer = read(idsock_servidor,buffer,100);		
		
		if(tamBuffer > 0)
		{
			strcpy(m.msn,buffer);
			tamBuffer = read(idsock_servidor,m.apodoDestinatario,20);					
			if(tamBuffer > 0)			
			{	
				m.apodoDestinatario[tamBuffer] ='\0';
				imprimirMensaje(&m);
			}	
		}
		
	}	
	

	return EXIT_SUCCESS;
}
