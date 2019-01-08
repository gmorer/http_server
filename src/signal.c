#include <signal.h>
#include "server.h"

int g_socket_sd;

void quit(int sig)
{
	printf("Terminate with signal %d\n", sig);
	close(g_socket_sd);
	exit(1);
}

void catch_sig( void )
{
	signal(SIGINT, &quit);
	signal(SIGABRT, &quit);
	signal(SIGBUS, &quit);
	signal(SIGALRM, &quit);
	signal(SIGFPE, &quit);
	signal(SIGHUP, &quit);
	signal(SIGILL, &quit);
	signal(SIGINT, &quit);
	signal(SIGKILL, &quit);
	//signal(SIGPIPE, &quit);
	signal(SIGPOLL, &quit);
	signal(SIGPROF, &quit);
	signal(SIGSEGV, &quit);
	signal(SIGSYS, &quit);
	signal(SIGTERM, &quit);
	signal(SIGTRAP, &quit);
	return ;
}