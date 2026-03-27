#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
	int value;
	sem_t sem1;
	sem_t sem2;
	int WhoWins;
} shared;

int main(){
	char* winner[] = {"remis","drugi", "pierwszy"};
	char* symbol[] = {"kamien", "papier","nozyce"};
	srand(time(NULL));
	shared * data = mmap ( NULL , sizeof ( shared ) , PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS ,-1 , 0);
	sem_init(&data-> sem1, 1, 0);
	sem_init(&data-> sem2, 1, 0);
	// 0 - kamien 1 - papier 2 - nozyce
	if(fork() == 0){
		sem_wait(&data->sem1);
		srand( (unsigned) time(NULL) * getpid());
		int temp = rand() % 3;
		printf("%s %s \n", symbol[data->value],symbol[temp]);
		int diff = (temp - data->value + 3) % 3;
		data->WhoWins = diff;
		sem_post(&data->sem2);
		exit(0);
	}	
	else {
		data->WhoWins = 1;
		data->value = (rand() % 3);
		sem_post(&data->sem1);
		sem_wait(&data->sem2);
		printf("Wynik: %s\n ", winner[data->WhoWins]);
		wait(NULL);
	}
	sem_destroy(&data->sem1);
	sem_destroy(&data->sem2);
	munmap(data,sizeof(shared));


	return 0;
}
