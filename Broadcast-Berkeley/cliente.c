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

#include <pthread.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

extern int mkaddr(
                  void *addr,
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

pthread_mutex_t tokenEscritura = PTHREAD_MUTEX_INITIALIZER ;
pthread_mutex_t token = PTHREAD_MUTEX_INITIALIZER ;

void foo(void * dato){
/*	while(1)
	{
	if( strcmp(buffer,"\0") !=0 ){
	//fflush(stdout);
	//printf("-----> buffer = [ %s ].\n",auxiliar);
	}
	} */
}

void funtionEscritura( void * dato )
{

	//do
	//{	
//		pthread_mutex_lock(&tokenEscritura);
		fflush(stdin);
	 	printf("\nPlease enter the message: ");
	 	bzero(buffer,256);
	 	bzero(auxiliar,256);
		fgets(buffer,255,stdin);
		strcpy(auxiliar,buffer);
		char * pch = strchr( auxiliar, 10);
		
		// Si la encontro, reemplazas nueva linea con nulo, que es el fin
		// de cadena ('')
		if (pch != NULL)
		{
		   *pch = ' ' ;
		}

		printf("Ingreso %s. \n" ,buffer);
//		pthread_mutex_unlock(&tokenEscritura);
//	}
//	while(strcmp(buffer,"exit") != 0 );	
}

int main(int argc,char **argv) {
     int z;
     int x;
     
     struct sockaddr_in adr;  /* AF_INET */
     int len_inet;            /* length */
     int s;                   /* Socket */
     char dgram[512];         /* Recv buffer */
     
     struct sockaddr_in  server_sock;
     int len_server;
     int IdServer;     
     char buff[100];		
     int finBuff;

     double diffServidor = 0 ;
     

     static int so_reuseaddr = TRUE;
     static char
     *bc_addr = "127.255.255.2:9097",
     *server_addr = "127.0.0.99:1234";
     time_t tiempo = time(NULL);	    
     time_t tiempoAnterior = 0; 

    /*
     * Use a server address from the command
     * line, if one has been provided.
     * Otherwise, this program will default
     * to using the arbitrary address
     * 127.0.0.:
     */
     if ( argc > 1 )/* Broadcast address: */
        bc_addr = argv[1];
     if (argc > 2)
	 server_addr = argv[2];    

    /*
     * Create a UDP socket to use:
     */
     s = socket(AF_INET,SOCK_DGRAM,0);
     if ( s == -1 )
        displayError("socket()");

    /*
     * Form the broadcast address:
     */
     len_inet = sizeof adr;

     z = mkaddr(&adr,
                &len_inet,
                bc_addr,
                "udp");

     if ( z == -1 )
        displayError("Bad broadcast address");

    /*
     * Allow multiple listeners on the
     * broadcast address:
     */
     z = setsockopt(s,
                    SOL_SOCKET,
                    SO_REUSEADDR,
                    &so_reuseaddr,
                    sizeof so_reuseaddr);

     if ( z == -1 )
        displayError("setsockopt(SO_REUSEADDR)");

    /*
     * Bind our socket to the broadcast address:
     */
     z = bind(s,
             (struct sockaddr *)&adr,
             len_inet);

     if ( z == -1 )
        displayError("bind(2)");

     /*
      * Configuro la conexion con el servidor TCP/IP
      */
     IdServer = socket(AF_INET,SOCK_STREAM,0);
     if(IdServer < 0)
	displayError("Socket() server");

     len_server = sizeof(server_sock);

     z = mkaddr(&server_sock , &len_server ,server_addr ,"tcp");
     if( z < 0)
	  displayError("Bad Server address");
     
     
     z = connect(IdServer,(struct sockaddr *)&server_sock,len_server);
     if(z < 0)
	  displayError("Connect() Server ");
     /*
      * Termino configurar la conexion con el servidor TCP 
      */

     //Lanzo un hilo para poder escribir
     pthread_t hiloEscritura,h2;
    //pthread_create( & hiloEscritura , NULL, (void *) funtionEscritura,(void *) NULL);	
    // pthread_create( &h2 ,NULL, (void *) foo,NULL);
     
     while ( 1 ) {
        /*
         * Wait for a broadcast message:
         */   
             
	     //pthread_create(&hiloEscritura, NULL, (void*) funtionEscritura,NULL);  
	     
     	
	     bzero(dgram,512);
     	     z = recvfrom(s,      /* Socket */
                      dgram,  /* Receiving buffer */
                      sizeof dgram,/* Max rcv buf size */
                      0,      /* Flags: no options */
                      (struct sockaddr *)&adr, /* Addr */
                      &x);    /* Addr len, in & out */

         	if ( z < 0 )
	      	displayError("recvfrom(2)"); /* else err */
		
		//printf(" %s \n", dgram);

		
		printf("Recibi del servidor %s\n",dgram);
     }
	 /*	time_t resultado = (time_t) strtod(dgram,NULL);
	  	if( resultado != tiempoAnterior)
		{	
			//Recive la señal de sincronizacion
			tiempoAnterior = resultado;
		
	 		diffServidor = difftime(resultado,tiempo );
			printf("Dentro del if antes del handshake ");	
			//Inicio el HandShake
			snprintf(buff,100,"%s","sync()");
			write(IdServer,buff,100 );
			// LEO LA RESPUESTA DEL CLIENTE ;	
			bzero(buff,100);
			read( IdServer, buff, 100);
			if ( strcmp(buff,"ok") == 0)
			{	
			     //ENVIO AL CLIENTE LA DIFERENCIA DE TIEMPO CON EL SERVIDOR
			     snprintf(buff,100,"%f",diffServidor);
	     		     write(IdServer,buff,100 );	
			     
			     bzero(buff,100);
			     read(IdServer,buff,100);
			     
			     diffServidor = (double) strtod(buff,NULL);
//			     write(IdServer,"exit",100);
		     	     printf("imprimo la diferencia con el servidor %.f \n",(double)diffServidor);	    
			} 
			*/
	//	}	 		
	//	pthread_join(hiloEscritura,NULL);
	 /*fwrite(dgram,z,1,stdout);
         putchar('\n');
/
 	fflush(stdout); */
 //    }

     fflush(stdout); 
     printf("Sali del WHILE LOCO\n");
     write(IdServer,"exit",100);
 
     close(s);
     //pthread_join(hiloEscritura,NULL);
     //pthread_join(h2,NULL);
     return 0;
}
