#include "ft_ping.h"

int check_args(char *arg) {
	if (arg[0] == '-')
		parse_options(arg[1]);
	else
		check_address(arg);
	return 0;
}

int parse_options(int key) {
	switch (key) {
		case 'h':
		case '?':
			printf("Usage: ft_ping [options] <hostname>\n");
			printf("Options:\n");
			printf("  -h\t\tDisplay this help message\n");
			printf("  -c <count>\tStop after sending <count> packets\n");
			printf("  -i <interval>\tWait <interval> seconds between sending packets\n");
			exit(0);
		case 'c':
			// Handle count option
			return 0;
		case 'i':
			// Handle interval option
			return 0;
		default:
			fprintf(stderr, "Unknown option: -%c\n", key);
			return -1;
	}
}

int check_address(char *hostname) {

	struct addrinfo hints, *res, *p;
	int status;

	memset(&hints, 0, sizeof hints); // Be sure the struct is empty
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_RAW; // Ping uses a socket raw

	if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo for '%s' failed : %s \n", hostname, gai_strerror(status));
		exit(1);
	}

	for (p = res; p != NULL; p = p->ai_next) {
		void *addr;
		char ipstr[INET_ADDRSTRLEN];

		// Get the pointer to the address itself
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
		addr = &(ipv4->sin_addr);

		// Convert the IP to a string and print it
		if (inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr) != NULL) {
			printf("Resolved address: %s\n", ipstr);
		}
		else {
			fprintf(stderr, "inet_ntop failed: %s\n", strerror(errno));
			freeaddrinfo(res);
			exit(1);
		}
	}
	
	freeaddrinfo(res); // Free the linked list

	printf("Pinging %s...\n", hostname);

	return 0; // Placeholder for address checking logic
}
