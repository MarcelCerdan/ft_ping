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

int check_address(char *str_address) {

	printf("Pinging %s...\n", str_address);

	struct in_addr *addr = malloc(sizeof(struct in_addr));
	if (addr == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}

	if (inet_aton(str_address, addr) == 0) { // Check if the address is a valid IPv4 address
		fprintf(stderr, "Invalid IPv4 address: %s\n", str_address);
		exit(1);
	}

	return 0; // Placeholder for address checking logic
}
