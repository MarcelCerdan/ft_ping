#include "ft_ping.h"

int send_ping(ping_data *data, struct timeval current_time) {

	build_icmp_packet(data);
		
	ssize_t bytes_sent = sendto(data->ping_fd, data->send_buffer, PING_PACKET_SIZE, 0,
		(struct sockaddr *)&data->dest_addr, sizeof(data->dest_addr));
	if (bytes_sent < 0) {
		perror("sendto");
		data->errors_received++;
		return 1;
	}
	else {
		data->packets_sent++;
		data->last_packet_time = current_time; // Update last packet time
		printf("Sent packet %d (%zd bytes)\n", data->ping_seq, bytes_sent);
		return 0;
	}
}

void recv_ping(ping_data *data, struct timeval current_time) {
	struct sockaddr_in recv_addr;
	socklen_t addr_len = sizeof(recv_addr);
	ssize_t bytes_received;

	bytes_received = recvfrom(data->ping_fd, data->recv_buffer, sizeof(data->recv_buffer), 0,
		(struct sockaddr *)&recv_addr, &addr_len);
	if (bytes_received < 0) {
		perror("recvfrom");
		data->errors_received++;
		return;
	}

	struct icmphdr *icmp_hdr = (struct icmphdr *)(data->recv_buffer + sizeof(struct iphdr)); // Skip IP header
	if (icmp_hdr->type == ICMP_ECHOREPLY && ntohs(icmp_hdr->un.echo.id) == data->ping_id) {
		data->packets_received++;
		struct timeval *tv_payload = (struct timeval *)(data->recv_buffer + ICMP_HEADER_SIZE);
		double rtt = (current_time.tv_sec - tv_payload->tv_sec) * 1000.0 + 
		             (current_time.tv_usec - tv_payload->tv_usec) / 1000.0; // RTT in milliseconds

		if (data->min_rtt < 0 || rtt < data->min_rtt) {
			data->min_rtt = rtt;
		}
		if (rtt > data->max_rtt) {
			data->max_rtt = rtt;
		}
		data->sum_rtt += rtt;

		printf("Received packet %d from %s: RTT = %.2f ms\n", ntohs(icmp_hdr->un.echo.sequence), 
		       inet_ntoa(recv_addr.sin_addr), rtt);
	} else {
		printf("Received non-echo reply or invalid packet\n");
		data->errors_received++;
	}
}