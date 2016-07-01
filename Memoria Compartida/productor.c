#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "operipc.h"

#define BUFFER_SIZE 10

char elemento (void);

int main() {
	int semid = semget(0XA,0,0);
	int shmbufid = shmget(0XA,0,0);
	int ent=0;
	int shmcontid = shmget (0XB,0,0);
	printf("Productor \n");
	printf("shmbufid %d shmcontid %d semid %d\n",shmbufid ,shmcontid,semid);

	char *dirbuf = (char *) shmat(shmbufid,0,0);
	int *dircon = (int *) shmat(shmcontid,0,0); 

	while (1) {
		sleep(1);
		while (*dircon==BUFFER_SIZE);
		dirbuf[ent]=elemento();
		write(1,&dirbuf[ent],1);
		P(semid,0);
		(*dircon)++; //seccion critica
		V(semid,0);
		if (ent==BUFFER_SIZE-1) printf("\n");
		ent=(ent+1)%BUFFER_SIZE;
	}
		
	shmdt(dirbuf);  
	return 0;
}

char elemento(void) {
	static char letra='A'-1;
	if (letra=='Z') letra='A';
	else letra++;
	return letra;
}