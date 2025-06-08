#include "ft_ping.h"

void create_socket(ping_data *data) {
	struct timeval tv;
    tv.tv_sec = 1;  // 1 second timeout for receiving
    tv.tv_usec = 0;

	printf("Creating raw socket for %s...\n", data->ip_str);
	// Create a raw socket
	data->ping_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (data->ping_fd < 0) {
		perror("socket");
		exit_clean(NULL, data, 1);
	}

	// Set the socket option to allow sending raw packets
	if (setsockopt(data->ping_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
		perror("setsockopt");
		exit_clean(NULL, data, 1);
	}

	printf("Socket created successfully with fd: %d\n", data->ping_fd);
}