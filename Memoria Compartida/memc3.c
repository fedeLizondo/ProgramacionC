#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h> 

//ejemplo lector
//leo el abecedario de la memoria compartida (de memc2.c)

int main()
{
	int shmid = shmget(0XA,0,0); //devuelve el shmid
	printf("shmid %d \n",shmid );
	sleep(1);
	char *dir = (char *) shmat(shmid,0,0); //mapeo el proceso con el recurso (para poder acceder)
	char *aux;
	aux=dir;
	int letra = *dir;
	while (letra<='Z') {
			write(1,&letra,1);
			usleep(200000);
			if (letra=='Z') break;
			dir++;
			letra = *dir;
		}

	shmdt(aux); //libero el recurso  
	return 0;
}
//scanf("%c%*c", &ch1);
