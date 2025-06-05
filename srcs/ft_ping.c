#include "ft_ping.h"

int main(int ac, char **av) {
	if (ac < 2) {
		printf("Usage: ft_ping <hostname>\n");
		return 1;
	}
	
	int i = 1;
	while (av[i]) {
		check_args(av[i]);
		i++;
	}
}