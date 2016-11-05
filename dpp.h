#ifndef DPP_H
#define DPP_H

#include <stdbool.h>
#include <pthread.h>

#define P 5
#define MAX_CYCLES 20

pthread_mutex_t mutex[P],disp_mutex;
pthread_cond_t forks[P];
pthread_t philosopher[P];
bool forkState[P];
int total_cycles = 0, phil_cycles[P];

void pickup_forks(int); 
void return_forks(int);
void *current_thread(void *);
bool display_count(int);

#endif
