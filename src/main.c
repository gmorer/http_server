#include "server.h"

int main(int argc, char **argv)
{
	if (!init_server(argc > 1 ? atoi(argv[1]) : 0))
	{
		write(2, "Cannot initialize the server\n", 29);
		return (1);
	}
	launch_server();
	return (0);
}
