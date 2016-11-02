// Compile using gcc dpp.c -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#define P 7

pthread_mutex_t mutex[P];
pthread_cond_t forks[P];
bool forkState[P];

void pickup_forks(int i) 
{
	pthread_mutex_lock(&mutex[i]);
	pthread_mutex_lock(&mutex[(i+1)%P]);

	while(!forkState[i])
		pthread_cond_wait(&forks[i],&mutex[i]);
	forkState[i] = false;

	while(!forkState[(i+1)%P])
		pthread_cond_wait(&forks[(i+1)%P],&mutex[(i+1)%P]);		
	forkState[(i+1)%P] = false;
	
	pthread_mutex_unlock(&mutex[i]);
	pthread_mutex_unlock(&mutex[(i+1)%P]);

}

void return_forks(int i) 
{
	pthread_mutex_lock(&mutex[i]);
	pthread_cond_signal(&forks[i]);
	forkState[i] = true;
	pthread_mutex_unlock(&mutex[i]);

	pthread_mutex_lock(&mutex[(i+1)%P]);
	pthread_cond_signal(&forks[(i+1)%P]);
	forkState[(i+1)%P] = true;
	pthread_mutex_unlock(&mutex[(i+1)%P]);

}

void *current_thread(void *philosopher_number)
 {
	while(1) {
		pickup_forks(*(int*)philosopher_number);
		printf("Philosopher %d is eating.\n",*(int*)philosopher_number + 1);

		usleep((rand()%3+1)*(1e6));
		
		return_forks(*(int*)philosopher_number);
		printf("Philosopher %d is thinking.\n",*(int*)philosopher_number + 1);	
		
		usleep((rand()%3+1)*(1e6));
	}
}

int main(int argc, char const *argv[]) 
{
	pthread_t philosopher[P];
	srand(time(NULL));

	int i,tmp[P],err;
	for(i = 0;i < P;++i) {
		forkState[i] = true;
		err = pthread_mutex_init(&mutex[i],NULL);
		assert(!err);
		err = pthread_cond_init(&forks[i],NULL);
		assert(!err);
	}

	//Launch threads. 
	for(i = 0;i < P;++i) {
		tmp[i] = i;
		printf("Starting thread %d\n", i);
		int err = pthread_create(&philosopher[i],NULL,(void*)current_thread,(void*)&tmp[i]);
		assert(!err);
	}

	for(i = 0;i < P;++i) 
		pthread_join(philosopher[i],NULL);

	for(i = 0;i < P;++i) {
		pthread_mutex_destroy(&mutex[i]);
		pthread_cond_destroy(&forks[i]);
	}


	return 0;
}