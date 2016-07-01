#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>

#include <pthread.h>
#include "mkaddr.c"
#include "lista.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
 
extern int mkaddr( void *addr, int *addrlen, char *str_addr, char *protocol);
 
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER ;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER ;


int cantidadMAXClientes;
int cantidadClientes;

double tiempoMAXClientes;

double totaldiffTiempo;
double promedioFinal;

double tiempoDeEspera;
time_t tiempoActual;
time_t tiempoFinal;

time_t tiempoServidor;

struct struct_idSockCliente{
	int idSockCliente;
};

/*
 * This function reports the error and
 * exits back to the shell:
 */
static void displayError(const char *on_what) {
	  fputs(strerror(errno),stderr);
	  fputs(": ",stderr);
	  fputs(on_what,stderr);
	  fputc('\n',stderr);
	  exit(1);
}

void funcionUnicast( void *);
void funtionLanzadorHilos(void *);

/* Datos del servidor TCP/IP GLOBALES PARA SER USADO POR LOS HILOS */
static char *sv_respuesta_addr = "127.0.0.99:1234";
struct sockaddr_in adr_respuesta;
int len_respuesta;
/*
lista * lEliminarClientes;
lista * listaClientes;
*/
int IdRespuestaSocket;
int cantidadAcalcular;
int cantidadAcalcularPreparados;

struct struct_idSockCliente mistruct;


int main(int argc,char **argv) {
	
	cantidadAcalcular = 0;
        cantidadAcalcularPreparados = 0;
	
	char bcbuf[512], *bp;/* Buffer and ptr */
	int z;      /* Status return code */
	int s;      /* Socket */
	
/*	listaClientes     = createList();	
	lEliminarClientes = createList();
*/
	struct sockaddr_in adr_srvr;/* AF_INET */
	int len_srvr;               /* length */
	  
	struct sockaddr_in adr_bc;  /* AF_INET */
	int len_bc;                 /* length */
	  
	static int so_broadcast = TRUE;
	  
	static char *sv_addr = "127.0.0:*",
	  	    *bc_addr = "127.255.255.2:9097";//DIRECCION DE BROADCAST
	
	
	/* Datos de servidor TCP/IP  */
//	static char *sv_respuesta_addr = "127.0.0.99:1234";//Direccion servidor tcp/ip 		
//	struct sockaddr_in adr_respuesta;
//      int len_respuesta;	
	/*Fin datos servidor*/

	//struct struct_idSockCliente mistruct;

	int argumento = 0 ;

	cantidadMAXClientes = 0;
	tiempoMAXClientes   = 0;
	
	tiempoServidor = time(NULL);

	for( argumento = 1;argumento < argc;argumento++ )
	{

		if( strcmp( argv[argumento],"-t") == 0 )
		{
			tiempoMAXClientes = strtod( argv[argumento +1] ,NULL );
		}
		if( strcmp( argv[argumento],"-c") == 0 ) 
		{
			cantidadMAXClientes =(int) strtol( argv[argumento+1],NULL,10 );
		}
		if(strcmp (argv[argumento],"-a") == 0 )
		{
			cantidadMAXClientes = 3 ;
		}

	}
	/*
	if ( argc > 2 )
	{
		//sv_addr = argv[2]; dejo direccion default
		//asigno por argumento , la direccion del servidor tcp/ip
		sv_respuesta_addr = argv[2];
	}

	if ( argc > 1 ) // Broadcast address: 
		bc_addr = argv[1];
	*/
	 /*
	  * Form the server address:
	  */
	len_srvr = sizeof adr_srvr;

	z = mkaddr(
		   &adr_srvr,  /* Returned address */
		   &len_srvr,  /* Returned length */
		   sv_addr,    /* Input string addr */
		   "udp");     /* UDP protocol */
	
	if ( z == -1 )
		displayError("Bad server address");
	 
	 /*
	  * Form the broadcast address:
	  */
	len_bc = sizeof adr_bc;
	 
	z = mkaddr(
		  &adr_bc, /* Returned address */
		  &len_bc, /* Returned length */
		  bc_addr, /* Input string addr */
		  "udp"); /* UDP protocol */
	 
	if ( z == -1 )
		displayError("Bad broadcast address");
	 
	/*
	 * respuesta del servidor
	 */
        len_respuesta = sizeof ( adr_respuesta );
        
	z = mkaddr ( &adr_respuesta , &len_respuesta ,sv_respuesta_addr,"tcp");	       
	if( z == -1)
		displayError("Bad server address");

	// Create a UDP socket to use:
	s = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if ( s < 0 )
		displayError("socket()");
	 
	/*
	 * Allow broadcasts:
	 */
	z = setsockopt(s,
	               SOL_SOCKET,
		       //SO_REUSEPORT,
		       SO_BROADCAST,
		       &so_broadcast,
		       sizeof(so_broadcast) );
	
	
	if ( z == -1 )
		displayError("setsockopt(SO_BROADCAST)");

	// Bind an address to our socket, so that client programs can listen to this server: /
	if( bind( s, (struct sockaddr *) &adr_srvr, len_srvr) < 0)
		displayError("bind()"); 
	 
	//INICIALIZO EL SERVIDOR TCP-IP PARA RECIBIR TIEMPOS DE LOS CLIENTES
	
	IdRespuestaSocket = socket(AF_INET,SOCK_STREAM,0);

	if( bind(IdRespuestaSocket,(struct sockaddr *) &adr_respuesta,len_respuesta) < 0)
		displayError("ERROR bind() respuesta socket");
	
	if( listen(IdRespuestaSocket,SOMAXCONN ) < 0)
		displayError("ERROR listen() respuesta socket");
	
	// CREO UN HILO PARA RECIBIR LA RESPUESTA DE LOS CLIENTES
	pthread_t CreadorHilos;
	pthread_create(&CreadorHilos ,NULL, (void *) funtionLanzadorHilos ,(void *) &mistruct );
	
	tiempoActual = time(NULL);

	while(1)//LOOP INFINITO
	{
		 
	         //CASTEO el tiempo del servidor a un string para enviar a los clientes	
		 sprintf( bcbuf , "%.f",(double) tiempoServidor);
		 
		 /* Broadcast the updated info:  */
		 z = sendto(s,	 
			    bcbuf,
			    strlen(bcbuf),
			    0,
			    (struct sockaddr *)&adr_bc,
			    len_bc );
		 
		 printf("Envie %s \n",bcbuf); 
		 if ( z == -1 ) //OCURRIO UN ERROR CON EL SENDTO
			  displayError("sendto()");
		 
		 sleep(1);//4);
	
	  }
	  pthread_join( CreadorHilos ,NULL);		 
	  return 0;
}

void funcionUnicast(void * dato)
{
	struct struct_idSockCliente *idSocketCliente =(struct struct_idSockCliente*) dato;
	
	char buff[100];
	
	int puedoCambiarVariable = 0;

	double diffTiempoSegundos = 0;
		
	int finBuffer = 0;
	
	int idSockCliente = idSocketCliente->idSockCliente;
	
	int cantidad = 100 ;

	do{
	        bzero(buff,100);
		finBuffer = read( idSockCliente,buff , 100 ) ;
		//printf("ESTOY EN EL WHILE finBuffer = %d \n y el buffer tiene %s\n ",finBuffer,buff);
		if( strcmp( buff,"sync()") == 0 )
		{			
			printf("DESPUES DEL OK\n");
			write(idSockCliente ,"ok",100);
			printf("ANTES DEL READ\n");
			bzero(buff,100);
			read(idSockCliente,buff,100);	
		        printf("Antes del MUTEX \n");	
			//BLOQUEO LAS VARIABLEAS HASTA QUE TERMINE DE SUMAR DIFERENCIA DEL CLIENTE
			pthread_mutex_lock(&m1);
				cantidadAcalcularPreparados++;
				diffTiempoSegundos = strtod(buff,NULL);
				totaldiffTiempo += diffTiempoSegundos ;
			pthread_mutex_unlock(&m1);
						
			printf("Despues del MUTEX socketId = %d \n",idSockCliente);	
			while( !promedioFinal ){} ;	
			
			snprintf(buff,100,"%lf",promedioFinal);
			write(idSockCliente,buff,100);
			printf("Envio el promedioFinal %.f \n",promedioFinal); 
			cantidadAcalcular++;		
		}	 
	}
	while( strcmp(buff,"exit") != 0  );
	printf("Sali del Hilo \n");
	close(idSockCliente);
	fflush(stdout);
	cantidad--;
	//pthread_t hAux = pthread_self();
	//push_back(lEliminarClientes,& hAux );
	pthread_exit(NULL);
}

void funtionLanzadorHilos(void * dato )
{
	while(1){
	
	if( cantidadClientes >= 0 &&  cantidadClientes < cantidadMAXClientes )
	{
		mistruct.idSockCliente = accept(
        	                                IdRespuestaSocket,
        	                                (struct sockaddr *)&adr_respuesta,
        	                                 &len_respuesta
        	                                );
        	if( mistruct.idSockCliente >= 0 )
        	{
        		printf("Conexion aceptada desde el cliente %d\n",mistruct.idSockCliente);
    		
			pthread_t hilo;
        //		push_back(listaClientes ,&hilo);
			cantidadClientes++;
        		pthread_create( &hilo,NULL,(void *)funcionUnicast,(void *)&mistruct);
        	}

	}
	else
	{
		if( cantidadAcalcular >= cantidadMAXClientes )
		{	
			printf("%d/%d\n",cantidadClientes,cantidadMAXClientes);
			printf("TERMINE DE CALCULAR EL PROMEDIO Y ENVIE EL RESULTADO\n");
			tiempoServidor+=promedioFinal;
			promedioFinal = 0;
			totaldiffTiempo = 0;
			cantidadAcalcular = 0;
			cantidadAcalcularPreparados = 0;
		}
		
		if( cantidadAcalcularPreparados >= cantidadMAXClientes )
		{
			printf("%d/%d\n",cantidadClientes,cantidadMAXClientes);
			printf("CALCULO PROMEDIO A DEVOLVER\n");
			promedioFinal = totaldiffTiempo / cantidadClientes;
			cantidadAcalcularPreparados = 0;
			cantidadAcalcular = 0;	

		}

	}
	

/*	while ( !isEmpty(lEliminarClientes))
	{	
		void * puntero;
	 	pop_back(lEliminarClientes,&puntero);
		pthread_t * hAux = (pthread_t *) puntero;
		pthread_join(*hAux,NULL);
	}
*/
	}
	pthread_exit(NULL);
}


