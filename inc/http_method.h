#ifndef HHTP_METHODE

# define HTTP_METHODE


# define MAX_HTTP_CODE 511

# define XX(a, b, c) (struct t_methode){a, b, c}

typedef struct	s_method {
	int	number;
	char	*name;
}				t_method;

# define METHODS_ARRAY {	\
	(t_method){100, "CONTINUE"},							\
	(t_method){101, "SWITCHING_PROTOCOLS"},				\
	(t_method){102, "PROCESSING"},						\
	(t_method){200, "OK"},								\
	(t_method){201, "CREATED"},							\
	(t_method){202, "ACCEPTED"},							\
	(t_method){203, "NON_AUTHORITATIVE_INFORMATION"},	\
	(t_method){204, "NO_CONTENT"},						\
	(t_method){205, "RESET_CONTENT"},					\
	(t_method){206, "PARTIAL_CONTENT"},					\
	(t_method){207, "MULTI_STATUS"},						\
	(t_method){208, "ALREADY_REPORTED"},					\
	(t_method){226, "IM_USED"},							\
	(t_method){300, "MULTIPLE_CHOICES"},					\
	(t_method){301, "MOVED_PERMANENTLY"},				\
	(t_method){302, "FOUND"},							\
	(t_method){303, "SEE_OTHER"},						\
	(t_method){304, "NOT_MODIFIED"},						\
	(t_method){305, "USE_PROXY"},						\
	(t_method){307, "TEMPORARY_REDIRECT"},				\
	(t_method){308, "PERMANENT_REDIRECT"},				\
	(t_method){400, "BAD_REQUEST"},						\
	(t_method){401, "UNAUTHORIZED"},						\
	(t_method){402, "PAYMENT_REQUIRED"},					\
	(t_method){403, "FORBIDDEN"},						\
	(t_method){404, "NOT_FOUND"},						\
	(t_method){405, "METHOD_NOT_ALLOWED"},				\
	(t_method){406, "NOT_ACCEPTABLE"},					\
	(t_method){407, "PROXY_AUTHENTICATION_REQUIRED"},	\
	(t_method){408, "REQUEST_TIMEOUT"},					\
	(t_method){409, "CONFLICT"},							\
	(t_method){410, "GONE"},								\
	(t_method){411, "LENGTH_REQUIRED"},					\
	(t_method){412, "PRECONDITION_FAILED"},				\
	(t_method){413, "PAYLOAD_TOO_LARGE"},				\
	(t_method){414, "URI_TOO_LONG"},						\
	(t_method){415, "UNSUPPORTED_MEDIA_TYPE"},			\
	(t_method){416, "RANGE_NOT_SATISFIABLE"},			\
	(t_method){417, "EXPECTATION_FAILED"},				\
	(t_method){421, "MISDIRECTED_REQUEST"},				\
	(t_method){422, "UNPROCESSABLE_ENTITY"},				\
	(t_method){423, "LOCKED"},							\
	(t_method){424, "FAILED_DEPENDENCY"},				\
	(t_method){426, "UPGRADE_REQUIRED"},					\
	(t_method){428, "PRECONDITION_REQUIRED"},			\
	(t_method){429, "TOO_MANY_REQUESTS"},				\
	(t_method){431, "REQUEST_HEADER_FIELDS_TOO_LARGE"},	\
	(t_method){451, "UNAVAILABLE_FOR_LEGAL_REASONS"},	\
	(t_method){500, "INTERNAL_SERVER_ERROR"},			\
	(t_method){501, "NOT_IMPLEMENTED"},					\
	(t_method){502, "BAD_GATEWAY"},						\
	(t_method){503, "SERVICE_UNAVAILABLE"},				\
	(t_method){504, "GATEWAY_TIMEOUT"},					\
	(t_method){505, "HTTP_VERSION_NOT_SUPPORTED"},		\
	(t_method){506, "VARIANT_ALSO_NEGOTIATES"},			\
	(t_method){507, "INSUFFICIENT_STORAGE"},				\
	(t_method){508, "LOOP_DETECTED"},					\
	(t_method){510, "NOT_EXTENDED"},						\
	(t_method){511, "NETWORK_AUTHENTICATION_REQUIRED"}	\
}

# undef XX

#endif
