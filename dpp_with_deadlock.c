#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include "dpp_with_deadlock.h"

int main(int argc, char const *argv[]) 
{
	srand(time(NULL));
	int i,tmp[P],err;

	err = pthread_mutex_init(&disp_mutex,NULL);
	assert(!err);

	for(i = 0;i < P;++i) {
		forkState[i] = true;
		phil_cycles[i] = 0;

		err = pthread_mutex_init(&mutex[i],NULL);
		assert(!err);
	}
	
	printf("# Eating Count = 0\n");

	//Launch threads. 
	for(i = 0;i < P;++i) {
		tmp[i] = i;
		
		// printf("Philosopher %d arrived at the table.\n", i+1);
		int err = pthread_create(&philosopher[i],NULL,(void*)current_thread,(void*)&tmp[i]);
		assert(!err);
	}

	for(i = 0;i < P;++i) {
		pthread_join(philosopher[i],NULL);	
	}

	for(i = 0;i < P;++i) {
		printf("Philosopher %d : %d\n", i+1,phil_cycles[i]);
	}

	pthread_mutex_destroy(&disp_mutex);
	for(i = 0;i < P;++i) {
		pthread_mutex_destroy(&mutex[i]);
	}

	return 0;
}

void pickup_forks(int i) 
{
	pthread_mutex_lock(&mutex[i]);	
	
	while(!forkState[i]);
	forkState[i] = false;
	
	pthread_mutex_unlock(&mutex[i]);

	pthread_mutex_lock(&mutex[(i+1)%P]);
	
	while(!forkState[(i+1)%P]);
	forkState[(i+1)%P] = false;
	
	pthread_mutex_unlock(&mutex[(i+1)%P]);

}

void return_forks(int i) 
{
	pthread_mutex_lock(&mutex[i]);
	forkState[i] = true;
	pthread_mutex_unlock(&mutex[i]);

	pthread_mutex_lock(&mutex[(i+1)%P]);
	forkState[(i+1)%P] = true;
	pthread_mutex_unlock(&mutex[(i+1)%P]);

}

bool display_count(int p) {
	bool end = false;
	pthread_mutex_lock(&disp_mutex);
	
	printf("# Eating Count = %d\n", ++total_cycles);
	
	if(total_cycles >= MAX_CYCLES)
		end = true;
	
	pthread_mutex_unlock(&disp_mutex);
	return end;
}

void *current_thread(void *philosopher_number)
 {
 	int p = *(int*)philosopher_number,delay;
	while(1) {
		printf("Philosopher %d waiting and trying to pickup forks.\n",p+1);
		pickup_forks(p);

		delay = (rand()%3+1);
		printf("Philosopher %d is eating for %d seconds.\n",p + 1, delay);
		sleep(delay);

		return_forks(p);
		
		delay = (rand()%3+1);
		printf("Philosopher %d is thinking for %d seconds.\n",p + 1, delay);	
		sleep(delay);
		
		phil_cycles[p]++;	
		if(display_count(p))
			break;
	}
}
