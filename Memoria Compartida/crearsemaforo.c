#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>


int main(int argc, char **argv)
{
	int semid = semget(0XA,1,IPC_CREAT|0600);
	printf("semid %d \n",semid);
	semctl(semid,0,SETVAL,1);
	return 	semctl(semid,0,GETVAL);
}

