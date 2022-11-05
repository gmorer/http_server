#ifndef _WORKER_H_
#define _WORKER_H_

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>


/*
	Design:

	main thread => read inputs
	workers: handle connection
*/

struct s_worker {
	pthread_t tid;
};

struct t_job {
	void (*work_fn)(void *);
	void *data;

	// Used by the queue
	struct t_job *_prev;
	struct t_job *_next;
};


bool init_workers();
bool work(void (*work_fn)(void *), void *data);

#endif
