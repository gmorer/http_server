#include "../inc/server.h"

int g_socket_fd;

void close_server(int sig) {
	close(g_socket_fd);
	printf("Terminate with signal %d\n", sig);
	exit(1);
}

void catch_sig( void )
{
	signal(SIGINT, &close_server);
	signal(SIGABRT, &close_server);
	signal(SIGBUS, &close_server);
	signal(SIGALRM, &close_server);
	signal(SIGFPE, &close_server);
	signal(SIGHUP, &close_server);
	signal(SIGILL, &close_server);
	signal(SIGINT, &close_server);
	signal(SIGKILL, &close_server);
	//signal(SIGPIPE, &close_server);
	signal(SIGPROF, &close_server);
	signal(SIGSEGV, &close_server);
	signal(SIGSYS, &close_server);
	signal(SIGTERM, &close_server);
	signal(SIGTRAP, &close_server);
	return ;
}

int init_server(int port, t_endpoint *endpoints)
{
	int one = 1;
	struct sockaddr_in server = {
	    .sin_family = AF_INET,
	    .sin_port = htons(port ? port : DEFAULT_PORT),
	    .sin_addr.s_addr = INADDR_ANY,
	};

	if ((g_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (0);
	/*---Assign a port number to the socket---*/
	if (bind(g_socket_fd, (struct sockaddr *) &server, sizeof(server)) != 0)
	{
		perror("socket--bind");
		return (0);
	}
	if (setsockopt(g_socket_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(g_socket_fd)) == -1)
	{
		perror("Setsockopt");
		return (0);
	}
	/*---Make it a "listening socket"---*/
	if (!compil_regex(endpoints))
		return (0);

	if (!init_workers())
		return (0);
	return (1);
}

void launch_server(void)
{
	int poll = epoll_create(1);

	if (poll == -1) {
		dprintf(2, "Event loop creation error.\n"); // Not really an event loop
		return ;
	}

	catch_sig();

	// Add socket fd to the poll

	struct epoll_event socket_event = {
		.events = EPOLLIN | EPOLLHUP | EPOLLET,
		.data.fd = g_socket_fd,
	};

	if (epoll_ctl(poll, EPOLL_CTL_ADD, g_socket_fd, &socket_event) != 0) {
		dprintf(2, "Failed to add the socket to the poll");
	}

	struct epoll_event events[POLL_BUFFER_LEN] = {};

	printf("listening or socket\n");
	if (listen(g_socket_fd, 20) != 0)
	{
		perror("socket--listen");
		return ;
	}

	int index = 0;
	printf("Launching epoll wait\n");
	while ((index = epoll_wait(poll, (struct epoll_event*)&events, POLL_BUFFER_LEN, -1)) != -1) {
		printf("incside index: %d\n", index);
		while (index != 0) {
			index -= 1;
			if (events[index].data.fd == g_socket_fd) {
				if (events[index].events & EPOLLHUP) {
					// Clear everything
					printf("event close soerver socket\n");
					return ;
				} else if (events[index].events & EPOLLIN) {
					// New client
					t_client *client = accept_client(g_socket_fd);
					printf("new client\n");
					if (!client) {
						ALLOCATION_ERROR;
						continue;
					}

					struct epoll_event client_event = {
						.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP | EPOLLET,
						.data.ptr = (void*)client,
					};
					if (epoll_ctl(poll, EPOLL_CTL_ADD, client->clientfd, &client_event) != 0) {
						dprintf(2, "error while adding client to the loop");
					}
				} else {
					panic("Impossible");
				}
			} else {
				t_client *client = events[index].data.ptr;
				if (events[index].events & (EPOLLHUP | EPOLLRDHUP )) {
					// remove client from poll
					epoll_ctl(poll, EPOLL_CTL_DEL, client->clientfd, NULL);
					printf("client disconnected %p", client);
					free_client(client);
				} else if (events[index].events & EPOLLIN) {
					// client ready for read
					printf("should read from the client\n");
					work(read_client, (void*)client);

				} else {
					panic("Impossible");
				}
				continue ;
			}
		}
	}
}
