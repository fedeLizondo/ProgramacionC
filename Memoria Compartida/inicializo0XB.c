#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h> 


//inicializar 0XB EN 0

int main()
{
	int shmid = shmget(0XB,0,0); 
	printf("shmid %d \n",shmid );
	int *dir = (int *) shmat(shmid,0,0); 
	*dir=0;
	shmdt(dir);
	
	return 0;
}

