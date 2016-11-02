#ifndef DPP_H
#define DPP_H

#include <stdbool.h>
#include <pthread.h>

#define P 5

pthread_mutex_t mutex[P];
pthread_cond_t forks[P];
pthread_t philosopher[P];
bool forkState[P];

void pickup_forks(int i); 
void return_forks(int i);
void *current_thread(void *);

#endif