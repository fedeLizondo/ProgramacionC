#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "operipc.h"

#define BUFFER_SIZE 10


int main() {
	int semid = semget(0XA,0,0);
	int shmbufid = shmget(0XA,0,0);
	int sal=0;
	int shmcontid = shmget (0XB,0,0);
	printf("Consumidor \n");
	printf("shmbufid %d shmcontid %d semid %d\n",shmbufid ,shmcontid,semid );

	char *dirbuf = (char *) shmat(shmbufid,0,0);
	int *dircon = (int *) shmat(shmcontid,0,0); 

	while (1) {
		sleep(1);
		while (*dircon==0);
		write(1,&dirbuf[sal],1);
		P(semid,0);
		(*dircon)--; //seccion critica
		V(semid,0);
		if (sal==BUFFER_SIZE-1) printf("\n");
		sal=(sal+1)%BUFFER_SIZE;
	}
		
	shmdt(dirbuf);  
	return 0;
}
