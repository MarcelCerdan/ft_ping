#include "ft_ping.h"

ping_data initialise_ping_data(void) {
	ping_data data;

	data.ping_fd = -1; // Initialize socket to an invalid value
	data.ping_id = getpid() & 0xFFFF; // Use the process ID as the ping identifier
	data.ping_seq = 0; // Initialize sequence number

	data.ping_verbose = 0; // Default to non-verbose mode
	data.ping_count = 0; // Default to sending unlimited packets
	data.ping_interval = 1; // Default interval of 1 second

	data.ping_hostname = NULL; // No hostname set initially
	memset(data.ip_str, 0, INET_ADDRSTRLEN); // Clear IP address string
	memset(&data.dest_addr, 0, sizeof(data.dest_addr)); // Clear destination address structure

	data.packets_sent = 0;
	data.packets_received = 0;
	data.errors_received = 0;
	data.min_rtt = -1.0; // Initialize min RTT to an invalid value
	data.max_rtt = -1.0; // Initialize max RTT to an invalid value
	data.sum_rtt = 0.0; // Initialize sum RTT to zero

	gettimeofday(&data.start_time, NULL); // Set start time
	gettimeofday(&data.last_packet_time, NULL); // Set last packet time

	return data;
}

int main(int ac, char **av) {
	if (ac < 2) {
		printf("Usage: ft_ping <hostname>\n");
		return 1;
	}
	
	ping_data data = initialise_ping_data();
	
	int i = 1;
	while (av[i]) {
		check_args(av[i], &data);
		create_socket(&data);
		printf("Pinging %s...\n", data.ping_hostname);
		clean_ping_data(&data);
		i++;
	}
}
