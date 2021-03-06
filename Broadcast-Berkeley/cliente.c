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


int main(int argc,char **argv) {
     srand(time(NULL));

     int z; //utilizado para retorno de funcion
     int tamServidor; 
     
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
     
     time_t tiempo = time(NULL)+ (rand()%100) ;	    
     printf("\n| Tiempo Inicial del Cliente : %.f |\n\n",(double) tiempo);
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
     s = socket( AF_INET,SOCK_DGRAM,IPPROTO_UDP);//0);
     
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
     z = bind(s ,(struct sockaddr *)&adr ,len_inet);
     if ( z == -1 )
        displayError("bind(2)");
     
     // Configuro la conexion con el servidor TCP/IP
     
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
     int EXIT = 1; //
     while ( EXIT ) {
        
             	//Espero a que me llegue un mensaje por Broadcast	   
             
	     	//Limpio el buffer
		bzero(dgram,512);
	     	//Recivo datos del cliente
		z = recvfrom(s,      		      /*ID del Socket */
             	             dgram,  	         /* buffer que almacena el mensaje del servidor */
             	             sizeof dgram,	      /* tamaño del buffer */
             	             0,      		      /* sin Flags  */
            	             (struct sockaddr *)&adr, /* direccion del servidor */
            	             &tamServidor );	      /* tamaño del la direccion del servidor */
			
		//printf("Buff ES %s \n ",dgram);
         	if ( z < 0 )
	      		displayError("recvfrom(2)"); /*Ocurrio un error*/
  		   		
	 	//Casteo el buffer a un struct tipo time_t
		time_t resultado = (time_t) strtod(dgram,NULL);

		if( resultado != tiempoAnterior)
		{
			//Recive la señal de sincronizacion
			tiempoAnterior = resultado;
	 		diffServidor = difftime(resultado,tiempo );// + (rand()%100);//Agrega diferencia
			
			printf("Envio el nro %.f\n",diffServidor);

			//Inicio el HandShake, enviando sync() al servidor
			bzero(buff,100);
			snprintf(buff,100,"%s","sync()");
			write(IdServer,buff,100 );

			// LEO LA RESPUESTA DEL CLIENTE ;	
			bzero(buff,100);
			printf("ANTES DEL READ\n");
			read( IdServer, buff, 100);
			printf("DESPUES DEL READ y leo el hermoso buff %s \n",buff);
			if ( strcmp(buff,"ok") == 0)
			{	
			     //ENVIO AL SERVIDOR LA DIFERENCIA DE TIEMPO CON EL SERVIDOR
			     bzero(buff,100);
			     snprintf(buff,100,"%f",diffServidor);
	     		     
			     write(IdServer,buff,100 );	
			     printf("ENVIE EL MENSAJE AL SERVIDOR \n");
			     //LEO LA RESPUESTA DEL SERVIDOR;
			     bzero(buff,100);
			     read(IdServer,buff,100);

			     //Casteo la respuesta al string 
			     diffServidor = (double) strtod(buff,NULL);
			     
			     //ACTUALIZO EL TIEMPO DEL SERVIDOR;
			     tiempo +=diffServidor;	
			     write(IdServer,"exit",100);
		     	     printf("------>imprimo la diferencia con el servidor %.f \n",(double)diffServidor);	    
			}
		}
     }
     //fflush(stdout); 
     write(IdServer,"exit",100); 
     close(s);
     return 0;
}


