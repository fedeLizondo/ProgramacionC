#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 5
#define SEM_S  0
#define SEM_N  1
#define SEM_E  2

//wait: while(sem <=0); sem = sem - 1;
void P(int semid, int sem){
	struct sembuf buf;
	buf.sem_num = sem;
	buf.sem_op  = -1;
	buf.sem_flg = 0;
	semop(semid,&buf,1); //system call para la operacion atomica wait
};

//signal: sem = sem + 1;
void V(int semid, int sem){
	struct sembuf buf;
	buf.sem_num = sem;
	buf.sem_op  = 1;
	buf.sem_flg = 0;
	semop(semid,&buf,1); //system call para la operacion atomica signal
};
