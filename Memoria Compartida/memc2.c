#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h> 

//ejemplo escritor
//escribir el abecedario en memc y que persista (cuando termine el proceso)

int main()
{
	int shmid = shmget(0XA,0,0); //devuelve el shmid
	printf("shmid %d \n",shmid );
	int letra ='0';
	char *dir = (char *) shmat(shmid,0,0); //mapeo el proceso con el recurso (para poder acceder)
	char *aux;
	aux=dir;
	while (letra<='Z') {
			*dir = letra;
			letra++;
			dir++;
		}
	shmdt(aux); //libero el recurso  
	return 0;
}

