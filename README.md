# HTTP_PARSER

__ExpressJs__ like http server in __C__ library with only two functions: `init_server()` and `start_server()`.
You need to use regex for the path matching.

## Example:
```c
int main(int argc, char **argv)
{
	t_endpoint endpoints[] = {
		(t_endpoint){"^/$", 0, &hello_world, 0},
		(t_endpoint){"^/sum/(.*)/(.*)$", 2, &sum_ep, 0},
		(t_endpoint){"/", 0, &error_404, 0},
		ENDPOINTS_END
	};

	if (!init_server(argc > 1 ? atoi(argv[1]) : 0, endpoints))
	{
		dprintf(2, "Cannot initialize the server\n");
		return (1);
	}
	launch_server();
	return (0);
```
see `test/main.c`

## compil

compile with: `make && gcc test/main.c -Iinc/ libserver.a -lpthread inc/http_server.h`

## roadmap
- [x] simple request
- [x] request parsing (thx to http_parser)
- [x] regex endpoint routing
- [ ] response header
- [ ] middleware
- [ ] https
