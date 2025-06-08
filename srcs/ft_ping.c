#include "ft_ping.h"

int main(int ac, char **av) {
	if (ac < 2) {
		printf("Usage: ft_ping <hostname>\n");
		return 1;
	}
	
	int i = 1;
	while (av[i]) {
		ping_data *data = malloc(sizeof(ping_data));
		check_malloc("Main", data);
		check_args(av[i], data);
		clean_ping_data(data);
		i++;
	}
}