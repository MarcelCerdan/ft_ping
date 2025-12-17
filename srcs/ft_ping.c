#include "ft_ping.h"

int g_running = 1; // Global flag to control the ping loop

void handle_sigint(int sig) {
	(void)sig; // Unused parameter
	g_running = 0; // Set the global running flag to 0 to stop the ping loop
}

ping_data initialise_ping_data(void) {
	ping_data data;

	data.ping_fd = -1; // Initialize socket to an invalid value
	data.ping_id = getpid() & 0xFFFF; // Use the process ID as the ping identifier
	data.ping_seq = 0; // Initialize sequence number

	data.ping_verbose = 0;
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
	data.sum_rtt_squared = 0.0; // Initialize sum of squared RTTs to zero

	gettimeofday(&data.start_time, NULL); // Set start time
	gettimeofday(&data.last_packet_time, NULL); // Set last packet time

	memset(data.send_buffer, 0, PING_PACKET_SIZE); // Clear the send buffer
	memset(data.recv_buffer, 0, sizeof(data.recv_buffer)); // Clear the receive buffer

	return data;
}

void ping_loop(ping_data *data) {
	signal(SIGINT, handle_sigint); // Set up signal handler for Ctrl+C

	while (g_running && (data->ping_count == 0 || (size_t)data->packets_sent < data->ping_count)) {
		struct timeval loop_start_time;
		gettimeofday(&loop_start_time, NULL); // Get the current time for the loop

		if (send_ping(data) == 0) // Send the ICMP Echo Request
		 	recv_ping(data); // Receive the ICMP Echo Reply
			
		struct timeval loop_end_time;
        gettimeofday(&loop_end_time, NULL); // Get time after send/recv attempt

        long elapsed_us = (loop_end_time.tv_sec - loop_start_time.tv_sec) * 1000000L +
                          (loop_end_time.tv_usec - loop_start_time.tv_usec);

        long required_delay_us = data->ping_interval * 1000000L; // Desired total interval in microseconds

		if ((size_t)data->packets_sent == data->ping_count) {
			clean_ping_data(data); // Clean up after the ping loop
			exit(0);
		}

        if (elapsed_us < required_delay_us) {
            long sleep_duration_us = required_delay_us - elapsed_us;
            // printf("Sleeping for %ld us...\n", sleep_duration_us); // Debug print
            usleep(sleep_duration_us);
		}
			
		data->ping_seq++;
	}

	clean_ping_data(data); // Clean up after the ping loop
	exit(0);
}

int main(int ac, char **av) {
	if (ac < 2) {
		printf("Usage: ft_ping <hostname>\n");
		return 1;
	}
	
	ping_data data = initialise_ping_data();
	av++; // Skip the program name
	
	while (*av) {
		printf("Processing argument: %s\n", *av); // Debug print
		if (check_args(av, &data) == 0) {
			create_socket(&data);
			printf("Pinging %s (%s): %lu data bytes\n", data.ping_hostname, data.ip_str, PING_PAYLOAD_SIZE);
			ping_loop(&data);
			clean_ping_data(&data);
		}
		else
			av++;
		av++;
	}
}

