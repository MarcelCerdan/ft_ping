#include "ft_ping.h"

int is_number(const char *str) {
	size_t i = 0;

	while (str[i]) {
		if (!isdigit(str[i]))
			return 0;
		i++;
	}
	return 1;
}