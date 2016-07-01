#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "mkaddr.c"
#include "operipc.h"
#include "mensaje.h"
#include "lista.h"

#include <pthread.h>

#define PUERTOLECTURA 6666
#define PUERTOESCRITURA 6667

//////////////////////////////////////////////////
//Estructuras Necesarias Para Memoria Compartida
//////////////////////////////////////////////////

//Identificador de Memoria Compartida
int idshm;

//Cola de mensajes

lista * cMensajes;
//Cola de clientes { idSockCliente,IP:PUERTO}
typedef struct tCliente
{
	char IP[21];
	int idEscritura;
	char Apodo[20];
	int idLectura;
	int lugar;
}tCliente;

lista * lClientes;
lista * lIDClientesLectores;//ALMACENA TODOS LOS CLIENTES QUE LEYERON AL OBJETO;

/////////////////////////////////////////////////////////////
//CONEXION AL SERVIDOR
/////////////////////////////////////////////////////////////
static char * sv_respuesta_addr = "127.0.0.99:1234";//DIRECCION DEL SERVIDOR
struct sockaddr_in adr_respuesta;
int len_respuesta;

struct struct_idSockCliente 
{
	int idSockCliente;
	char IP[20];
};

struct struct_idSockCliente mistruct;
struct struct_idSockCliente misEscritura;

extern int mkaddr( void *addr,int *addrlen,char *str_addr,char *protocol);

#ifndef TRUE
	#define TRUE 1
	#define FALSE 0
#endif

static void displayError(const char *on_what) {
	  fputs(strerror(errno),stderr);
 	  fputs(": ",stderr);
  	  fputs(on_what,stderr);
  	  fputc('\n',stderr);
 	  exit(1);
}

///////////////////////////////////////////////////
//HILOS
////////////////////////////////////////////////////

int IDLECTOR;//0 SI ES TODOS ,-1 NADIE,!0 EL LECTOR 
int NUMERO;
int flagLectura;

void * hGestiona(void * dato);//HILO PRINCIPAL GESTOR DE LECTURA;

void hPrincipalLectura(void*);//HILO PRINCIPAL LECTURA
void hLectura(void *);

void hPrincipalEscritura(void*);//HILO PRINCIPAL ESCRITURA	
void hEscritura(void *);


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main(int argc,char **argv)
{
	//CREO UN AREA DE MEMORIA COMPARTIDA
	if(argc > 1)
	idshm = atoi(argv[1]);
	else
	idshm = shmget(0XB,sizeof(struct mensaje),IPC_CREAT|IPC_EXCL|0600);
	printf("SHARED MEMORY ID %d \n",idshm );	
	
	mensaje * msn = (struct mensaje *) shmat(idshm,0,0);
	msn->estado = OCUPADO;
	strcpy((*msn).msn,"HOLA MUNDO LOCO");
	imprimirMensaje(msn);
	
	flagLectura = TRUE;	

	//CREO LISTA Y COLA DE MENSAJE
	lClientes = (lista *)createList();
	lIDClientesLectores = (lista *)	createList();
	cMensajes = (lista *)createList();	
	
	tCliente relleno;
	strcpy(relleno.Apodo,"TODOS");
	relleno.lugar = 0; 	
	push_back(lClientes,(void*) &relleno);	
	
	pthread_t Escritura,
		  Lectura,
		  Gestora;

	//HILO DE ESCRITURA		 
	pthread_create(&Escritura,NULL,(void*)hPrincipalEscritura,NULL);
	//HILO DE LECTURA
	pthread_create(&Lectura,NULL,(void*) hPrincipalLectura,NULL);
	//HILO DE GESTION
	pthread_create(&Gestora,NULL,(void*) hGestiona,NULL);

	pthread_join(Escritura,NULL);//TERMINO EL HILO DE ESCRITURA
	pthread_join(Lectura,NULL);  //TERMINO EL HILO DE LECTURA
	pthread_join(Gestora,NULL);  //TERMINO EL HILO DE GESTION

	shmdt(msn);
	return EXIT_SUCCESS;
}

void hPrincipalLectura(void * dato)
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
	servidor_sock.sin_port        = htons(6667);
	servidor_sock.sin_addr.s_addr = inet_addr("127.0.0.99");
	
	memset(servidor_sock.sin_zero,0,8);
	
	printf(" Bind   %s]"ANSI_COLOR_RESET"\n",bind(
					idsock_servidor,              		\
			   		(struct sockaddr *) &servidor_sock,		\
			   		length_sock
				) == 0?ANSI_COLOR_GREEN"[OK":ANSI_COLOR_RED"[ERROR"
			);

	printf(" Listen %s]"ANSI_COLOR_RESET"\n",listen(idsock_servidor,5)==0?ANSI_COLOR_GREEN
		       "[OK":ANSI_COLOR_RED"[ERROR");
	
	fflush(stdout);
	for(;;)
	{
		printf("(LECTURA)Esperando conexion nro :%d\n",NUMERO);
		/*
		mistruct.idSockCliente = accept( idSocket,
					 (struct sockaddr*) &cliente_sock,
					  &len_srvr);
		*/
		mistruct.idSockCliente = accept(idsock_servidor ,		  
				        (struct sockaddr *)&cliente_sock, 
					&length_sock);
		
		if(mistruct.idSockCliente > 0 )
		{
			pthread_t hilo;
			pthread_create(&hilo,NULL,(void*)hLectura,(void*) &mistruct);
		}
		else
		{
			printf("Conexion Rechazada\n");
		}
	}
}

void hLectura(void * dato)
{
	struct struct_idSockCliente sCliente;
	struct struct_idSockCliente * sAuxCliente;
	sAuxCliente = (struct struct_idSockCliente*)dato;
	sCliente.idSockCliente = (*sAuxCliente).idSockCliente;
	fflush(stdout);	

	int id = sCliente.idSockCliente;
	int posicionEnLista = 0;
	
	char buffer[100];
	char apodoCliente[20];
	int finBuffer;
	
	mensaje * m =  (struct mensaje *) shmat(idshm,0,0) ;
	////////////////////////////////////////////////////
	//INICIO UN HANDSHAKE PARA IDENTIFICAR AL CLIENTE //
	////////////////////////////////////////////////////
	//RECIVO LA ID DEL CLIENTE
	bzero(buffer,100);	
	finBuffer = read(id,buffer,100);
	buffer[finBuffer]='\0';
	posicionEnLista = atoi(buffer); //ID DEL CLIENTE
	printf("LA POSICION EN LISTA ES %d\n",posicionEnLista);fflush(stdout);
	
	//BUSCO EL APODO DEL CLIENTE	
	startNext(lClientes);
		 	punt r = NULL;
			r = next( lClientes);
			while( r!=NULL )
			{	
				tCliente * aux = (struct tCliente *)r->data;
				if (aux->lugar == posicionEnLista)			
					strcpy(apodoCliente , aux->Apodo);
				r = next(lClientes);
			}	

	////ENVIO EL APODO DEL CLIENTE
	write(id,apodoCliente,20);
	
	for(;;)
	{
		if( IDLECTOR == 0 && lClientes->length > 1 && lIDClientesLectores->length+1>= lClientes->length ) 
		{
			m->estado = DISPONIBLE;	
			clear(lIDClientesLectores);
		}

		if( ( IDLECTOR == 0  || m->destino == posicionEnLista) && m->estado == OCUPADO && flagLectura == TRUE ) 		
		{			
		     if(IDLECTOR == 0)
		     {
			
			startNext(lIDClientesLectores);
		 	punt r = NULL;
			r = next( lIDClientesLectores);
			while( r!=NULL && (*(int*)r->data)!= m->destino)
			{	
				r = next(lClientes);
			}
			if(r ==NULL)
			{
				write(id,m->msn,100);		
				write(id,m->apodoDestinatario,20);
				push_back(lIDClientesLectores,(void*)&m->apodoDestinatario);			
			}
					 
		     }
		     else				
		     {	
			write(id,m->msn,100);		
			write(id,m->apodoDestinatario,20);		
			m->estado = DISPONIBLE;		
		     }
		}
		
		
		usleep(1000000);
	}	
//	while( strcmp(buffer,"exit()") );
	
	close(id);
	pthread_exit(NULL);		
}

void hPrincipalEscritura(void* dato ) 
{
	
	struct sockaddr_in adr;
	int estado;
	int len_inet;
	int idSocket;
	char buffer[100];
	int finBuffer;
	
	struct sockaddr_in adr_srvr;
	struct sockaddr_in cliente_sock;	

	int len_srvr = sizeof(adr_srvr);	
	static char *sv_addr = "127.0.0.99:6666";
	
	len_srvr = sizeof(adr_srvr);

	estado = mkaddr(&adr_srvr,&len_srvr,sv_addr,"udp");
	if(estado < 0)
		displayError("Direccion Servidor Erronea");
	
	idSocket = socket(AF_INET,SOCK_STREAM,0 );
	
	if(idSocket < 0)
		displayError("ERROR EN SOCKET()");

	if( bind(idSocket,(struct sockaddr *)&adr_srvr,len_srvr) < 0 )
	       displayError("ERROR EN BIND()");	

	if(listen(idSocket,SOMAXCONN ) < 0)
		displayError("ERROR LISTEN()Escritura\n ");
	for(;;)
	{
		
		misEscritura.idSockCliente = accept( idSocket,
					 (struct sockaddr*) &cliente_sock,
					  &len_srvr);
		
		if(misEscritura.idSockCliente > 0 )
		{
			pthread_t hilo;
			pthread_create(&hilo,NULL,(void*)hEscritura,(void*) &misEscritura);
		}
		else
		{
			printf("Conexion Rechazada %d \n");
		}
	}
}

void hEscritura(void * dato) 
{
		
	struct struct_idSockCliente sCliente;
	struct struct_idSockCliente * sAuxCliente;
	
	sAuxCliente = (struct struct_idSockCliente*)dato;
	sCliente.idSockCliente = (*sAuxCliente).idSockCliente;
	
	int id = sCliente.idSockCliente;

	char buffer[100];
	int finBuffer;
	
	mensaje * m =  (struct mensaje *) shmat(idshm,0,0);
	
	/////////////////////////////////////////////////////////
	//	HANDSHAKE CON EL CLIENTE		       //
	/////////////////////////////////////////////////////////
        
	tCliente Cliente;
	
	//RECIVO APODO DEL CLIENTE
	bzero(buffer,100);	
	finBuffer = read(id,&Cliente.Apodo,100);//buffer,100);
	//}while(finBuffer > 0 );
	//	Cliente.Apodo[finBuffer]='\0';
		
	int idLectura;
	Cliente.lugar = (*lClientes).length;//LUGAR ES IGUAL A ID,LUGAR EN LA LISTA
        
	//ENVIAR COD IDENTIFICADOR DEL CLIENTE  	
	snprintf(buffer,100,"%d",Cliente.lugar);
	write(id,buffer,100);

	push_back(lClientes,(void*)&Cliente);
	
	mensaje * mRecivido,maux;
	
	for(;;){
		mRecivido = (struct mensaje*) malloc(sizeof(mensaje));
		//RECIVO ID DEL DESTINATARIO		
		bzero(buffer,100);
		finBuffer = read(id,buffer,100);

		if(finBuffer > 0 )//SI NO HAY ERROR
		{
			//CASTING DE BUFFER A idLectura
			buffer[finBuffer]='\0';				
			idLectura = atoi(buffer);
				
			bzero(buffer,100);
			finBuffer = read(id,(*mRecivido).msn,80 );
			mRecivido->msn[finBuffer] ='\0';			
			
			(*mRecivido).estado = OCUPADO;
			strcpy((*mRecivido).apodoDestinatario,Cliente.Apodo);
			(*mRecivido).destinatario = Cliente.lugar;
			(*mRecivido).destino = idLectura;
			
			//BUSCO EL APODO DEL DESTINO
			startNext(lClientes);
		 	punt r = NULL;
			r = next( lClientes);
			while( r!=NULL )
			{	
				tCliente * aux = (struct tCliente *)r->data;
				if (aux->lugar == idLectura)			
					strcpy( mRecivido->apodoDestino , aux->Apodo);
				r = next(lClientes);
			}	

			//GUARDO EN LA COLA DE MENSAJES
			push_back(cMensajes,mRecivido);
			
		
			
		}
		usleep(100000);	
	}	
			
		
}

//HILO QUE GESTIONA LA COLA Y LA MEMORIA PARA ESCRITURA
void * hGestiona(void * dato)
{	
	mensaje * m =  (struct mensaje *) shmat(idshm,0,0);;
	mensaje * men = NULL;
	punt r = NULL;
	tCliente * c;
	tCliente * aux;

	for(;;)
	{
		
		if(  IDLECTOR == 0 && lIDClientesLectores->length > 0 && ((lIDClientesLectores->length+1) >= lClientes->length))
		{	
			clear(lIDClientesLectores);
			m->estado = DISPONIBLE;
		}

		if( m->estado == DISPONIBLE && cMensajes->length > 0 )
		{	
			
			flagLectura = TRUE;
			void * puntero = NULL;
			r = NULL;			
			c = NULL;
			aux = NULL;
			men = NULL;
			
			pop_front(cMensajes,&puntero);			
			men = (struct mensaje *) puntero;

			strcpy( m->msn , men->msn);			
			m->destinatario = men->destinatario;
			m->destino = men->destino;
			strcpy(m->apodoDestinatario ,men->apodoDestinatario) ;
			strcpy(m->apodoDestino,men->apodoDestino);
			IDLECTOR = men->destino;
			
			//m->destino %=lClientes->length ;
			
			m->estado = OCUPADO;
			
			//free(men);	
		}

		if( m->estado == DISPONIBLE && cMensajes->length == 0 )
		{
			//FLAG INFORMANDO QUE NO LEA POR QUE NO HAY NADA
			flagLectura = FALSE;
		}
		usleep(1000000);//TODO ELIMINAR
	}	
}

