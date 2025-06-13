#include "ft_ping.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int check_args(char *arg, ping_data *data) {
	if (arg[0] == '-') {
		int i = 1;
		while (arg[i]) {
			parse_options(arg[i]);
			i++;
		}
		return 1;
	}
	else {
		check_address(arg, data);
		return 0;
	}
}

void parse_options(int key) {
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
			break;
		case 'i':
			// Handle interval option
			break;
		default:
			fprintf(stderr, "Invalid option: -%c\n", key);
			fprintf(stderr, "Use -h or --help for usage information.\n");
			exit(1);
	}
}

void check_address(char *hostname, ping_data *data) {

	struct addrinfo hints, *res, *p;
	int status;

	memset(&hints, 0, sizeof hints); // Be sure the struct is empty
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_RAW; // Ping uses a raw socket

	if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo for '%s' failed : %s \n", hostname, gai_strerror(status));
		exit_clean(NULL, data, 1);
	}
	
	if (res == NULL) {
    	fprintf(stderr, "No address found for '%s'\n", hostname);
    	exit_clean(NULL, data, 1);
	}

	for (p = res; p != NULL; p = p->ai_next) {
		void *addr;

		// Get the pointer to the address itself
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
		addr = &(ipv4->sin_addr);

		// Convert the IP to a string and print it
		if (inet_ntop(p->ai_family, addr, data->ip_str, sizeof data->ip_str) != NULL) {
			printf("Resolved address: %s\n", data->ip_str);
			data->ping_hostname = strdup(hostname);
			check_malloc("ping_hostname", data->ping_hostname, data);
			data->dest_addr = *ipv4; // Store the destination address
		}
		else {
			fprintf(stderr, "inet_ntop failed: %s\n", strerror(errno));
			freeaddrinfo(res);
			exit_clean(data, NULL, 1);
		}
	}

	freeaddrinfo(res);
}

