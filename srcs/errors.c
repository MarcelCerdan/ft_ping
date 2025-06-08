#include "ft_ping.h"

void check_malloc(char *error_msg, void *addr) {
	if (addr == NULL) {
		fprintf(stderr, "Memory allocation failed in '%s' \n", error_msg);
		exit_clean(NULL);
	}
}

void clean_ping_data(ping_data *data) {
	if (data) {
		if (data->ping_hostname) {
			free(data->ping_hostname);
		}
		if (data->ping_buffer) {
			free(data->ping_buffer);
		}
		if (data->ping_fd >= 0) {
			close(data->ping_fd);
		}
		free(data);
	}
}

void exit_clean(void *data_to_free, ping_data *data, int exit_code) {
	clean_ping_data(data);
	if (data_to_free) {
		free(data_to_free);
	}

	exit(exit_code);
}
