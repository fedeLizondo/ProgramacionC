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

int IDCLIENTE;
char apodo[20];
char apodoServer[20];


int main(int argc,char **argv){
	
	
	
	system("clear");
	
	printf("BIENVENIDO AL CHAT\n");
	printf("INGRESE UN USERNAME:\n");
	scanf("%s",apodo);
	fflush(stdin);
	system("clear");	
	

	struct sockaddr_in servidor_sock;

	int idsock_servidor, 
	    length_sock = sizeof(struct sockaddr_in);
	    	    	
	idsock_servidor = socket(AF_INET,SOCK_STREAM,0);
	      			
	printf("ID Socket Servidor %d \n",idsock_servidor);
	    
	servidor_sock.sin_family      = AF_INET;
	servidor_sock.sin_port        = htons(6666);
	servidor_sock.sin_addr.s_addr = inet_addr("127.0.0.99");	
	memset(servidor_sock.sin_zero,0,8);	    
		
	if( connect( idsock_servidor, (struct sockaddr *)&servidor_sock,sizeof(struct sockaddr_in)) ==-1)
	{
		printf(ANSI_COLOR_RED"ERROR al conectar el socket.\n"ANSI_COLOR_RESET);
		exit(1);
	}
	
	//////////////////////////////////////////////////	
	//	HANDSHAKE CON EL CLIENTE		//
	//////////////////////////////////////////////////	
	char mensaje[80];
        int IDDESTINO = 0;
	

	//ENVIO EL APODO AL SERVIDOR
	write(idsock_servidor,apodo,sizeof(apodo));

	//RECIVO COD IDENTIFICADOR DEL CLIENTE	
	bzero(buffer,100);
	int finBuffer = read(idsock_servidor,buffer,100 );	
	IDCLIENTE = atoi(buffer);//sprintf( buffer, "%d", IDCLIENTE );//CASTING DE Char * a INT
	printf("-------->RECIBO DEL SERVIDOR %s %d \n",buffer,IDCLIENTE );fflush(stdout);
	
	///////////////////////////////////////////////////////////	
	//	        ENVIAR AL clienteEscritura               //
	/////////////////////////////////////////////////////////// 			
	char consola[80];
	bzero(buffer,100);
	strcpy(consola,"gnome-terminal -e ./cLectura ");
	//strcpy(consola,"x-terminal-emulator -e ./cLectura ");	
	sprintf( buffer, "%d", IDCLIENTE );
	strncat(consola, buffer ,14);
	//strcpy(consola,"");
	system( consola );
	//printf(consola);
	system("clear");
		
	////////////////////////////////////////////////////////////
	//		INICIO EL ENVIO DE MENSAJES	          //
	////////////////////////////////////////////////////////////

	int exit = 1;
	while( exit )
	{	
		exit = 1;
		bzero(buffer,100);
		printf("Ingrese : ID a enviar mensaje ( 0 = TODOS,exit() salir ):\n");
		
		fflush(stdin);
		scanf("%s",buffer);
		exit = strcmp(buffer,"exit()"); 
		if( exit == 0 )
		{
			write(idsock_servidor,buffer,100);
			exit = atoi(buffer);			
		}
		else  
		{
			printf("Ingrese Mensaje:\n");
			scanf("%s",mensaje);
			fflush(stdout);
			fflush(stdin);
			write(idsock_servidor,buffer,100);			
			write(idsock_servidor,mensaje,70 );
		}	
	}
	
	return EXIT_SUCCESS;
	
}



