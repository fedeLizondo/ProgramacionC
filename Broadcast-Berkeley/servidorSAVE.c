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

int cantidad;

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

lista * listaClientes;

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
void funcionCreadoraHilos(void *);

static char *sv_respuesta_addr = "127.0.0.99:1234";
struct sockaddr_in adr_respuesta;
int len_respuesta;


int main(int argc,char **argv) {
	
	char bcbuf[512], *bp;/* Buffer and ptr */
	int z;      /* Status return code */
	int s;      /* Socket */
	
	listaClientes = createList();	

	struct sockaddr_in adr_srvr;/* AF_INET */
	int len_srvr;               /* length */
	  
	struct sockaddr_in adr_bc;  /* AF_INET */
	int len_bc;                 /* length */
	  
	static int so_broadcast = TRUE;
	  
	static char *sv_addr = "127.0.0:*",
	  	    *bc_addr = "127.255.255.2:9097";//DIRECCION DE BROADCAST
	
	
	/* Datos de servidor TCP/IP  */
	static char *sv_respuesta_addr = "127.0.0.99:1234";//Direccion servidor tcp/ip 		
	struct sockaddr_in adr_respuesta;
        int len_respuesta;	
	/*Fin datos servidor*/

	struct struct_idSockCliente mistruct;

	int argumento ;
	cantidadMAXClientes = 0;
	tiempoDeEspera = 0;
	
	for( argumento = 1;argumento < argc;argumento++ )
	{

		if( strcmp( argv[argumento],"-t") == 0 )
		{
			tiempoDeEspera = strtod( argv[argumento +1] ,NULL );
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
	
	if ( argc > 2 )
	{
		//sv_addr = argv[2]; dejo direccion default
		//asigno por argumento , la direccion del servidor tcp/ip
		sv_respuesta_addr = argv[2];
	}

	if ( argc > 1 ) /* Broadcast address: */
		bc_addr = argv[1];
	
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


	/*
	 * Create a UDP socket to use:
	*/
	s = socket(AF_INET,SOCK_DGRAM,0);
	if ( s == -1 )
		displayError("socket()");
	 
	/*
	 * Allow broadcasts:
	 */
	z = setsockopt(s,
	               SOL_SOCKET,
		       SO_BROADCAST,
		       &so_broadcast,
		       sizeof(so_broadcast) );
	 
	if ( z == -1 )
		displayError("setsockopt(SO_BROADCAST)");

	/*
	 * Bind an address to our socket, so that
	 * client programs can listen to this
	 * server:
	 */
	z = bind( s,
		  (struct sockaddr *)&adr_srvr,
		  len_srvr);
	 
	if ( z == -1 )
		displayError("bind()"); 
	 


	int IdRespuestaSocket = socket(AF_INET,SOCK_STREAM,0);
	if( bind(IdRespuestaSocket,(struct sockaddr *) &adr_respuesta,len_respuesta) < 0)
		displayError("ERROR bind() respuesta socket");
	if( listen(IdRespuestaSocket,5) < 0)
		displayError("ERROR listen() respuesta socket");
	
	
	while(1) {
		 
		  bp = bcbuf;
			
		  sprintf( bcbuf , "%.f",(double) time(NULL) );
	 	  printf("Envio este tiempo a los clientes %.f .\n",bcbuf );	
		  
		  /*
		  * Broadcast the updated info:
		  */
		
	         	  
		  z = sendto(s,	 bcbuf,
				 strlen(bcbuf),
				 0,
				(struct sockaddr *)&adr_bc,
				len_bc ); 
		  if ( z == -1 )
			  displayError("sendto()");
			
		  printf("Despues del sendto \n");
		  tiempoActual = time(NULL);
		  
		  mistruct.idSockCliente = accept(
			           		IdRespuestaSocket,
	                               		(struct sockaddr *)&adr_respuesta,
                                         	&len_respuesta
                                        	);
			
		  printf("Despues del accept\n ");
                  if( mistruct.idSockCliente >= 0 )
                  {
                         printf("Conexion aceptada desde el cliente %d\n",
                         mistruct.idSockCliente);
                         
			 pthread_t hilo;
	                 push_back(listaClientes ,&hilo);
                         
			 cantidadClientes++;
                         pthread_create( &hilo , NULL, (void *) funcionUnicast, (void *) &mistruct );
                  }
			
                  lista	* listAuxiliar = createList();	  
		  void * aux;
		  pthread_t h;
/*		  while(!isEmpty(listaClientes))
		  {
			 		
		         pop_front(listaClientes,&aux);
		 	 h = *( (pthread_t *) aux) ;	 
			 push_back (listAuxiliar,&h );

		  }	  */
		  
		 sleep(1);
		 printf("Despues del sleep\n");
		 fflush(stdout); 
	  }
			 
	  return 0;
}

void funcionUnicast(void * dato)
{
	struct struct_idSockCliente *idSocketCliente =(struct struct_idSockCliente*) dato;
	
	char buff[100];
	
	cantidad++;
	
	int puedoCambiarVariable = 0;

	double diffTiempoSegundos = 0;
		
	time_t tiempoCliente;

	int finBuffer = 0;
	
	int idSockCliente = idSocketCliente->idSockCliente;
	
	int cantidad = 100 ;

	do{
	        bzero(buff,100);
		finBuffer = read( idSockCliente,buff , 100 ) ;
		printf("ESTOY EN EL WHILE finBuffer = %d \n y el buffer tiene %s\n ",finBuffer,buff);
		if( strcmp( buff,"sync()") == 0 )
		{
		
			write(idSockCliente ,"ok",100);
			read(idSockCliente,buff,100);	
			printf("LEO DEL CLIENTE -> : %s \n",buff);
			//BLOQUEO LAS VARIABLEAS HASTA QUE TERMINE DE ESCRIBIR
			pthread_mutex_lock(&m1);
			
			diffTiempoSegundos = strtod(buff,NULL);
			totaldiffTiempo += diffTiempoSegundos ;
			
			pthread_mutex_unlock(&m1);
			
//			while( promedioFinal == 0){} ;	
			
			snprintf(buff,100,"%lf",diffTiempoSegundos);
			write(idSockCliente,buff,100);		
			read(idSockCliente ,buff,100);
		}	 
	}
	while( strcmp(buff,"exit") != 0  );
	printf("Sali del Hilo \n");
	close(idSockCliente);
	fflush(stdout);
	cantidad--;
}
/*
void funtionLanzadorHilos(void *)
{

}
*/

