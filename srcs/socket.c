#include "ft_ping.h"

void create_socket(ping_data *data) {
	struct timeval tv;
    tv.tv_sec = data->timeout;  // Timeout in seconds
    tv.tv_usec = 0;

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

	if (data->ttl_val > 0) {
        if (setsockopt(data->ping_fd, IPPROTO_IP, IP_TTL, &data->ttl_val, sizeof(data->ttl_val)) < 0) {
            perror("setsockopt ttl");
            exit_clean(NULL, data, 1);
        }
    }
}