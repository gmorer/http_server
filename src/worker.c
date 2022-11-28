#include "../inc/worker.h"
#include "../inc/utils.h"

#define CPU_PER_WORKER 1

struct s_worker (*g_workers)[] = NULL;
long g_proc_num = 0;

/* Shared by all the thread, linked to the condition */
pthread_mutex_t g_mutex_cond = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cond_worker = PTHREAD_COND_INITIALIZER;

/* queue */
pthread_mutex_t g_mutex_queue = PTHREAD_MUTEX_INITIALIZER;
struct t_job *g_queue_head = NULL;
struct t_job *g_queue_tail = NULL;

void _enqueue(struct t_job *job) {
	assert(job != NULL);
	job->_next = NULL;
	pthread_mutex_lock(&g_mutex_queue);
	if (g_queue_head == NULL) {
		assert(g_queue_tail == NULL);
		job->_prev = NULL;
		g_queue_head = job;
		g_queue_tail = job;
	} else {
		assert(g_queue_tail != NULL);
		job->_prev = g_queue_tail;
		g_queue_tail->_next = job;
		g_queue_tail = job;
	}
	pthread_mutex_unlock(&g_mutex_queue);
}

struct t_job *_dequeue() {
	pthread_mutex_lock(&g_mutex_queue);
	if (g_queue_head == NULL) {
		assert(g_queue_tail == NULL);
		return NULL;
	}
	struct t_job *job_res = g_queue_tail;
	if (job_res->_prev != NULL) {
		g_queue_tail = job_res->_prev;
		g_queue_tail->_next = NULL;
	} else {
		g_queue_tail = NULL;
		g_queue_head = NULL;
	}
	pthread_mutex_unlock(&g_mutex_queue);
	job_res->_prev = NULL;
	job_res->_next = NULL;
	return job_res;
}

void *_wait_for_work(void *this) {
	// TODO: properly kill thoses
	
	struct s_worker *self = (struct s_worker*)this;

	// Attach the worker the the cpu
	cpu_set_t *cpu_set = CPU_ALLOC(CPU_PER_WORKER);
	if (cpu_set == NULL) {
		ALLOCATION_ERROR;
		return NULL;
	}
	size_t cpu_set_size = CPU_ALLOC_SIZE(CPU_PER_WORKER);
	CPU_ZERO_S(cpu_set_size, cpu_set);
	CPU_SET_S(self->cpu_id, cpu_set_size, cpu_set);

	while (1) {
		/* wait for job to pop */
		pthread_cond_wait(&g_cond_worker, &g_mutex_cond);

		/* retrieve the job */
		struct t_job *local_work = _dequeue();
		if (local_work == NULL) {
			continue;
		}
		local_work->work_fn(local_work->data);

		free(local_work);
	}
	return NULL;
}

bool init_workers() {
	if (g_workers != NULL) {
		/* already initialised */
		return false;
	}
	g_proc_num = sysconf(_SC_NPROCESSORS_ONLN);
	/* No single thread mod yet */
	if (g_proc_num == 0 || g_proc_num == 1) {
		return false;
	}
	g_workers = calloc(g_proc_num, sizeof(struct s_worker));
	if (g_workers == NULL) {
		return false;
	}

	pthread_mutex_lock(&g_mutex_cond);
	pthread_attr_t workers_attr;
	if (pthread_attr_init(&workers_attr) != 0) {
		return false;
	}
	/* wont be joined */
	if (pthread_attr_setdetachstate(&workers_attr, PTHREAD_CREATE_DETACHED) != 0) {
		return false;
	}

	long i = 0;
	while (i < g_proc_num) {
		(*g_workers)[i].cpu_id = i;
		pthread_create(&((*g_workers)[i].tid), &workers_attr, _wait_for_work, (void*)&(*g_workers)[i]);
		i += 1;
	}

	pthread_attr_destroy(&workers_attr);

	return true;
}

// Add work_fn to the queue of work
bool work(void (*work_fn)(void *), void *data) {
	struct t_job *job = malloc(sizeof(*job));

	if (job == NULL) {
		return false;
	}
	job->data = data;
	job->work_fn = work_fn;
	_enqueue(job);
	pthread_cond_signal(&g_cond_worker);
	return true;
}
