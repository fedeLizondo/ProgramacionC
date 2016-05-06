#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "fecha.h" 

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
	
	memset(servidor_sock.sin_zero,0,8);
	
	////////////////////Start-Time-Server/////////////////
	
	tiempo_t tiempoServidor, 
		 tiempoCliente;

	dameTime(&tiempoServidor); 
	printf("Server ",tiempoServidor.t);
    	imprimeHora(tiempoServidor.tMaquina);
	
	/////////////////////Start-Server/////////////////////

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
			//pthread_t hilo1;
			//pthread_create(&hilo1,NULL,(void *) foo,NULL);
		}

	}

//	char b[128] ;	
//	DameFecha(b);
//	printf("HOLA MUNDO %s.\n",b);
	return EXIT_SUCCESS;
}

void funcionHilo(void * dato)
{
	//struct struct_idsockc * mia //Carga del struct pasado por parametro 
	char buff[30];
	time_t tiempoClienteSeg;
	int nb = 0;
	int idSockCliente = 0 ;
	
	do
	{
		//Leo desde el cliente
		nb = read(idSockCliente,buff,30);
		buff[nb] = "\0" ;		
		printf("------> Date Cliente %d : % s\n",idSockCliente,buff);
		time_t tiempoClienteSeg = strtod( buff, NULL);
		//Calculo diferencia con el tiempo actual 
		dameTime(&tiempoServidor);//Actualizo tiempo servidor		
		write(idSockCliente,difftime( tiempoServidor.t,
					      tiempoClienteSeg,
					      sizeof(double) );
		wait(1000);
	}
	while(strcmp(buff,"exit"));
	close(idSockCliente);
	pthread_exit(NULL);
}


