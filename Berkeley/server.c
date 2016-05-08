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

#define IP "127.0.0.1"//"192.168.1.103"
#define PUERTO 6666

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


struct struct_idSockCliente { int idSockCliente; }; 

void funcionHilo(void *);

int main(int argc,char * argv[])  
{	
	
	struct sockaddr_in servidor_sock,
			   cliente_sock;

	struct struct_idSockCliente mistruct;

	int idsock_servidor, // codigo identificador de socket del lado de servidor
	    //idsock_cliente,  // codigo identificador de socket del lado del client
	    length_sock = sizeof(struct sockaddr_in);
	
	idsock_servidor = socket(AF_INET,SOCK_STREAM,0);
	
	printf("ID Socket Servidor %d \n",idsock_servidor);

	servidor_sock.sin_family      = AF_INET;
	servidor_sock.sin_port        = htons(PUERTO);
	servidor_sock.sin_addr.s_addr = inet_addr(IP);
	
	memset(servidor_sock.sin_zero,0,8);
	
	pthread_t hilos[100];
	int cantidadHilos = 0;	
	////////////////////Start-Time-Server/////////////////
	
	tiempo_t * ts = malloc(sizeof(tiempo_t));
	tiempo_t tiempoServidor;  
	dameTime( ts  ); 
	tiempoServidor = *ts;
	printf("Server ");
    	imprimeHora(tiempoServidor.tMaquina);
	
	
	/////////////////////Start-Server/////////////////////

	printf("EL PID del Servidor es :  %d \n",getpid());
	
	printf("Bind %d\n",bind(idsock_servidor,              		\
			   (struct sockaddr *) &servidor_sock,		\
			   length_sock));

	printf(" Listen %s"ANSI_COLOR_RESET"\n",listen(idsock_servidor,5)==0?ANSI_COLOR_GREEN
		       "OK":ANSI_COLOR_RED"ERROR");

	while( 1 )//Infinity LOOP
	{
		printf("Esperando conexion %d .\n",cantidadHilos);

		mistruct.idSockCliente = accept(idsock_servidor ,		  \
				        (struct sockaddr *)&cliente_sock, \
					&length_sock);

		if( mistruct.idSockCliente != -1 )
		{
			printf("Conexion aceptada desde el cliente \n");
			//pthread_t hilo1;
			pthread_create(&hilos[++cantidadHilos],NULL,(void*) funcionHilo, (void*) &mistruct );
		}
		else
		{
			printf("Conexion rechazada %d \n");
		}
	}

	return EXIT_SUCCESS;
}

void funcionHilo(void * dato)
{
	//Cargo struct pasado por parametro
	struct struct_idSockCliente * idSocketCliente = (struct struct_idSockCliente *) dato;
      	 
	tiempo_t tiempoServidor;
	
	char buff[100];
	char buffer[100];
	
	time_t tiempoClienteSeg;
	
	int nb = 0;
	
	int idSockCliente = idSocketCliente->idSockCliente;	
	
	do
	{	//Leo desde el cliente
		nb = read(idSockCliente ,buff,100);
		buff[nb] = '\0';		
						
		time_t tiempoClienteSeg = (time_t) strtod( buff, NULL);

		printf("ID %d  Los datos del buff %s .\n",idSockCliente,buff);		
		//Calculo diferencia con el tiempo actual 	
		dameTime(&tiempoServidor);        	
		snprintf(
			buffer,100,"%lf",difftime( tiempoServidor.t, tiempoClienteSeg )
		       	);
	
		//Envio differencia en segundos 
		write( idSockCliente, buffer, sizeof(buffer) );
		
		//sleep(1);

	}
	while(strcmp(buff,"exit"));
	close(idSockCliente);
	pthread_exit(NULL);
}


