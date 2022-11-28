#include "../inc/io.h"
#include "../inc/server.h"

/*
 * System call wrappers provided since glibc does not yet
 * provide wrappers for io_uring system calls.
 * */

int io_uring_setup(unsigned entries, struct io_uring_params *p) {
	return (int) syscall(__NR_io_uring_setup, entries, p);
}

int io_uring_enter(int ring_fd, unsigned int to_submit,
		unsigned int min_complete, unsigned int flags) {
	return (int) syscall(__NR_io_uring_enter, ring_fd, to_submit,
			min_complete, flags, NULL, 0);
}

int io_setup_uring(struct io_event_queue *ring) {
	struct io_uring_params p;

	/* See io_uring_setup(2) for io_uring_params.flags you can set */
	memset(&p, 0, sizeof(p));
	int ring_fd = io_uring_setup(20 /* queue depth */, &p);
	if (ring_fd < 0) {
		perror("io_uring_setup");
		return 1;
	}

	/*
	 * io_uring communication happens via 2 shared kernel-user space ring
	 * buffers, which can be jointly mapped with a single mmap() call in
	 * kernels >= 5.4.
	 */

	int sring_sz = p.sq_off.array + p.sq_entries * sizeof(unsigned);
	int cring_sz = p.cq_off.cqes + p.cq_entries * sizeof(struct io_uring_cqe);

	/* Rather than check for kernel version, the recommended way is to
	 * check the features field of the io_uring_params structure, which is a
	 * bitmask. If IORING_FEAT_SINGLE_MMAP is set, we can do away with the
	 * second mmap() call to map in the completion ring separately.
	 */
	if (p.features & IORING_FEAT_SINGLE_MMAP) {
		if (cring_sz > sring_sz)
			sring_sz = cring_sz;
		cring_sz = sring_sz;
	}

	/* Map in the submission and completion queue ring buffers.
	 *  Kernels < 5.4 only map in the submission queue, though.
	 */
	void *sq_ptr = mmap(0, sring_sz, PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_POPULATE,
			ring_fd, IORING_OFF_SQ_RING);
	if (sq_ptr == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	void *cq_ptr = sq_ptr;
	if (!(p.features & IORING_FEAT_SINGLE_MMAP)) {
		/* Map in the completion queue ring buffer in older kernels separately */
		cq_ptr = mmap(0, cring_sz, PROT_READ | PROT_WRITE,
				MAP_SHARED | MAP_POPULATE,
				ring_fd, IORING_OFF_CQ_RING);
		if (cq_ptr == MAP_FAILED) {
			perror("mmap");
			return 1;
		}
	}
	/* Save useful fields for later easy reference */
	unsigned *sring_tail = sq_ptr + p.sq_off.tail;
	unsigned *sring_mask = sq_ptr + p.sq_off.ring_mask;
	unsigned *sring_array = sq_ptr + p.sq_off.array;

	/* Map in the submission queue entries array */
	void *sqes = mmap(0, p.sq_entries * sizeof(struct io_uring_sqe),
			PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE,
			ring_fd, IORING_OFF_SQES);
	if (sqes == MAP_FAILED) {
		perror("mmap");
		return 1;
	}
	/* Save useful fields for later easy reference */
	unsigned *cring_head = cq_ptr + p.cq_off.head;
	unsigned *cring_tail = cq_ptr + p.cq_off.tail;
	unsigned *cring_mask = cq_ptr + p.cq_off.ring_mask;
	void *cqes = cq_ptr + p.cq_off.cqes;

	ring->ring_fd = ring_fd;
	ring->sqes = sqes;
	ring->sring_tail = sring_tail;
	ring->sring_mask = sring_mask;
	ring->sring_array = sring_array;

	ring->cqes = cqes;
	ring->cring_tail = cring_tail;
	ring->cring_mask = cring_mask;
	ring->cring_head = cring_head;


	return 0;
} 
