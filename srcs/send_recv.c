#include "ft_ping.h"

int send_ping(ping_data *data) {
	struct timeval send_time;

	build_icmp_packet(data);

	
	ssize_t bytes_sent = sendto(data->ping_fd, data->send_buffer, PING_PACKET_SIZE, 0,
		(struct sockaddr *)&data->dest_addr, sizeof(data->dest_addr));
		if (bytes_sent < 0) {
			perror("sendto");
			data->packets_sent_errors++;
			return 1;
		}
		else {
			gettimeofday(&send_time, NULL);
			data->packets_sent++;
			data->last_packet_time = send_time; // Update last packet time
			printf("Sent packet %d (%zd bytes)\n", data->ping_seq, bytes_sent);
			return 0;
	}
}

void recv_ping(ping_data *data) {
	struct sockaddr_in recv_addr;
	socklen_t addr_len = sizeof(recv_addr);
	ssize_t bytes_received;
	struct timeval recv_time;

	while (1) {
		bytes_received = recvfrom(data->ping_fd, data->recv_buffer, sizeof(data->recv_buffer), 0,
			(struct sockaddr *)&recv_addr, &addr_len);
		
		gettimeofday(&recv_time, NULL); // Get the current time when receiving the packet

		if (bytes_received < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				printf("Request timed out for packet %d\n", data->ping_seq);
				data->packets_lost++;
				return;
			}
			perror("recvfrom");
			data->errors_received++;
			return; // Exit on error
		}

		struct iphdr *ip_hdr = (struct iphdr *)data->recv_buffer;
        int ip_header_len = ip_hdr->ihl * 4;

        if (ip_hdr->version != 4 || (size_t)bytes_received < (size_t)ip_header_len + sizeof(struct icmphdr)) {
            // Not IPv4 or too small, discard and wait for another one
            printf("Received non-IPv4 or too small packet. Discarding.\n"); // Debug
            continue; // Continue waiting for a valid packet
        }

		struct icmphdr *icmp_hdr = (struct icmphdr *)(data->recv_buffer + ip_header_len);
		
		if (icmp_hdr->type == ICMP_ECHOREPLY && ntohs(icmp_hdr->un.echo.id) == data->ping_id && ntohs(icmp_hdr->un.echo.sequence) == (unsigned short)(data->ping_seq)) {
			data->packets_received++;

			struct timeval *send_time_payload = (struct timeval *)(data->recv_buffer + ip_header_len + sizeof(struct icmphdr));

			long rtt_us = (recv_time.tv_sec - send_time_payload->tv_sec) * 1000000L +
                          (recv_time.tv_usec - send_time_payload->tv_usec);
            double rtt_ms = (double)rtt_us / 1000.0;

			if (rtt_ms < 0)
				rtt_ms = 0.0;

			if (data->min_rtt < 0 || rtt_ms < data->min_rtt)
				data->min_rtt = rtt_ms;

            if (rtt_ms > data->max_rtt)
				data->max_rtt = rtt_ms;

            data->sum_rtt += rtt_ms;

			char ip_source_str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(ip_hdr->saddr), ip_source_str, sizeof(ip_source_str));

			printf("%zd bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
                   bytes_received - ip_header_len,
                   ip_source_str,
                   ntohs(icmp_hdr->un.echo.sequence),
                   ip_hdr->ttl,
                   rtt_ms);
            return; // Successfully processed OUR packet, exit recv_ping
		}
	}
}