#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include  "fechaHora.h"

#define IP "192.168.1.103"
#define PUERTO 6666

int main( ) 
{	
	
	struct sockaddr_in servidor_sock,
			   cliente_sock;
	
	int idsock_servidor, // codigo identificador de socket del lado de servidor
	    idsock_cliente,  // codigo identificador de socket del lado del client
	    length_sock = sizeof(struct sockaddr_in);
	
	idsock_servidor = socket(AF_INET,SOCK_STREAM,0);
	printf("ID Socket Servidor %d\n",idsock_servidor);

	servidor_sock.sin_family      = AF_INET;
	servidor_sock.sin_port        = htons(PUERTO);
	servidor_sock.sin_addr.s_addr = inet_addr(IP);
	
	memset(servidor_sock.sin_zero,0,8);//
	
	//printf("EL PID del Servidor es :  %d \n",getpid());
	
	printf("Bind %d\n",bind(idsock_servidor,              		\
			   (struct sockaddr *) &servidor_sock,		\
			   length_sock));

	printf("Listen %d\n",listen(idsock_servidor,5));

	while( 1 )//Infinity LOOP
	{
		printf("Esperando conexion.\n");

		idsock_cliente = accept(idsock_servidor ,		  \
				        (struct sockaddr *)&cliente_sock, \
					&length_sock);

		if( idsock_cliente != -1 )
		{
			printf("Conexion aceptada desde el cliente \n");
		}			
	}

	char b[128] ;	
	DameFecha(b);
	printf("HOLA MUNDO %s.\n",b);
	return EXIT_SUCCESS;
}
