#include "ft_ping.h"

void check_malloc(char *error_msg, void *addr) {
	if (addr == NULL) {
		fprintf(stderr, "Memory allocation failed in '%s' \n", error_msg);
		exit_clean(NULL);
	}
}

void exit_clean(void *data_to_free) {
	if (data_to_free) {
		free(data_to_free);
	}

	exit(1);
}
