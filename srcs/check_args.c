#include "ft_ping.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int check_args(char **av, ping_data *data) {
	if (av[0][0] == '-') {
		parse_options(av, data);
		return 1;
	}
	else {
		check_address(*av, data);
		return 0;
	}
}

void parse_options(char **av, ping_data *data) {
	int i = 1;
	
	while (av[0][i] != '\0') {
		char key = av[0][i++];

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
				av++;
				if (!is_number(*av))
					error_msg("Invalid argument for -c option. Must be a number.", data);
				data->ping_count = strtoul(*av, NULL, 10);
				return;
			case 'i':
				av++;
				if (!is_number(*av))
	 				error_msg("Invalid argument for -i option. Must be a number.", data);
				data->ping_interval = strtoul(*av, NULL, 10);
				return;
			default:
				fprintf(stderr, "Invalid option: -%c\n", key);
				fprintf(stderr, "Use -h for usage information.\n");
				exit(1);
		}
	}

}

void check_address(char *hostname, ping_data *data) {
	printf("Checking address for hostname: %s\n", hostname); // Debug print

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
			//printf("Resolved address: %s\n", data->ip_str); // Debug print
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

