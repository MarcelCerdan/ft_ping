#include "ft_ping.h"

unsigned short in_cksum(unsigned short *addr, int len) {
    int sum = 0;
    unsigned short *w = addr;
    unsigned short answer = 0;

    while (len > 1) {
        sum += *w++;
        len -= 2;
    }

    if (len == 1) {
        *(unsigned char *)&answer = *(unsigned char *)w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}

void build_icmp_packet(ping_data *data) {
	memset(data->send_buffer, 0, PING_PACKET_SIZE);
	
	struct icmphdr *icmp_hdr = (struct icmphdr *)data->send_buffer; // Pointer to the ICMP header in the send buffer

	icmp_hdr->type = ICMP_ECHO; // Set ICMP type to Echo Request
	icmp_hdr->code = 0; // Code is 0 for Echo Request
	icmp_hdr->un.echo.id = htons(data->ping_id); // Set the identifier
	icmp_hdr->un.echo.sequence = htons(data->ping_seq); // Set the sequence number

	struct timeval *tv_payload = (struct timeval *)(data->send_buffer + ICMP_HEADER_SIZE); // Pointer to the payload in the send buffer
	gettimeofday(tv_payload, NULL); // Get the current time for the payload
	
	printf("Time payload set: %ld.%06ld seconds\n", tv_payload->tv_sec, tv_payload->tv_usec);

	icmp_hdr->checksum = in_cksum((unsigned short *)data->send_buffer, PING_PACKET_SIZE); // Calculate the checksum

	printf("ICMP packet built with ID: %d, Sequence: %d\n", ntohs(icmp_hdr->un.echo.id), ntohs(icmp_hdr->un.echo.sequence));
}