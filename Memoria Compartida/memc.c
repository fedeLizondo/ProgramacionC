#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h> //libreria para memc
#include <sys/shm.h> //libreria para memc

//$ ipcs (enter) por consola
//crea dos areas de memoria compartida (para comunicacion entre productor-consumidor)


int main()
{
	int shmbufid = shmget(0XA,32,IPC_CREAT|IPC_EXCL|0600);
	int shmcontid = shmget(0XB,sizeof(int),IPC_CREAT|IPC_EXCL|0600);
	printf("shmbufid %d \n",shmbufid );  //imprime el shmid
	printf("shmcontid %d \n",shmcontid ); //imprime el shmid
	return 0;
}