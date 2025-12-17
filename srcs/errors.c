#include "ft_ping.h"

void check_malloc(char *error_msg, void *addr, ping_data *data) {
	if (addr == NULL) {
		fprintf(stderr, "Memory allocation failed in '%s' \n", error_msg);
		exit_clean(NULL, data, 1);
	}
}

void clean_ping_data(ping_data *data) {

	if (data) {
		print_statistics(data); // Print final statistics
		if (data->ping_fd != -1) {
			close(data->ping_fd); // Close the socket if it was opened
		}
		if (data->ping_hostname) {
			free(data->ping_hostname); // Free the hostname if it was allocated
		}
		memset(data, 0, sizeof(ping_data)); // Clear the structure
		data->ping_fd = -1; // Reset socket to an invalid value
		data->ping_id = getpid() & 0xFFFF; // Reset ping identifier to current process ID
		data->ping_seq = 0; // Reset sequence number
		data->packets_sent = 0;
		data->packets_received = 0;
		data->errors_received = 0;
		data->min_rtt = -1.0; // Reset min RTT to an invalid value
		data->max_rtt = -1.0; // Reset max RTT to an invalid value
		data->sum_rtt = 0.0; // Reset sum RTT to zero
		gettimeofday(&data->start_time, NULL); // Reset start time
		gettimeofday(&data->last_packet_time, NULL); // Reset last packet time
	}
}

void exit_clean(void *data_to_free, ping_data *data, int exit_code) {
	clean_ping_data(data);
	if (data_to_free) {
		free(data_to_free);
	}

	exit(exit_code);
}

void error_msg(char *msg, ping_data *data) {
	fprintf(stderr, "%s\n", msg);
	exit_clean(NULL, data, 1);
}