#ifndef _IO_H_
#define _IO_H_

#include <linux/io_uring.h>
#include <sys/mman.h>
#include <sys/syscall.h>

struct io_event_queue {
	int ring_fd;

	/* Submissions part */
	struct io_uring_sqe *sqes;
	unsigned *sring_tail;
	unsigned *sring_mask;
	unsigned *sring_array;

	/* Completion part */
	struct io_uring_cqe *cqes;
	unsigned *cring_tail;
	unsigned *cring_mask;
	unsigned *cring_head;
};

int io_setup_uring(struct io_event_queue *ring);

#endif
