#include "ft_ping.h"

int main(int ac, char **av) {
	if (ac < 2) {
		printf("Usage: ft_ping <hostname>\n");
		return 1;
	}

	printf("Pinging %s...\n", av[1]);
}