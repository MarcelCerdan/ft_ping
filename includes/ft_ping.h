#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include <sys/time.h>
# include <sys/types.h>
# include <sys/socket.h>

# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>

# include <errno.h>

# define PING_PACKET_SIZE 64 // Size of the ICMP packet to send
# define ICMP_HEADER_SIZE sizeof(struct icmphdr) // Size of the ICMP header
# define PING_PAYLOAD_SIZE (PING_PACKET_SIZE - ICMP_HEADER_SIZE) // Size of the payload in the ICMP packet

# define RECV_PACKET_SIZE 2048 // Size of the buffer for receiving packets

typedef struct ping_data
{
	int ping_fd;					/* Raw socket */
	int ping_id;					/* Ping identifier */
	unsigned short ping_seq;		/* Sequence number of the packet */

	// Options
	int ping_verbose;				/* Verbose mode */
	size_t ping_count;				/* Number of packets to send */
	size_t ping_interval;			/* Number of second to wait between packets */

	// Target information
	char *ping_hostname;			/* Printable hostname */
	char ip_str[INET_ADDRSTRLEN];	/* Resolved IP address string */
	struct sockaddr_in dest_addr;	/* Destination address structure for sendto() */

	 // Statistics
    long packets_sent;               /* Number of ICMP Echo Requests sent */
    long packets_received;           /* Number of ICMP Echo Replies received */
    long errors_received;            /* Number of error packets received (e.g., unreachable) */
    double min_rtt;                  /* Minimum Round Trip Time */
    double max_rtt;                  /* Maximum Round Trip Time */
    double sum_rtt;                  /* Sum of RTTs for average calculation */

	// Timing
    struct timeval start_time;       /* Start time of the ping utility */
    struct timeval last_packet_time; /* Time when the last packet was sent (for interval) */

	// Buffer for sending/receiving packets
    char send_buffer[PING_PACKET_SIZE]; // Define PING_PACKET_SIZE
    char recv_buffer[2048];             // Sufficiently large buffer for receiving packets


} ping_data;

// --- FUNCTIONS PROTOTYPES --- //

// Parse and check arguments
int check_args(char *arg, ping_data *data);
void parse_options(int key);
void check_address(char *address, ping_data *data);

// Handling errors, memory and exit
void clean_ping_data(ping_data *data);
void check_malloc(char *error_msg, void *addr, ping_data *data);
void exit_clean(void *data_to_free, ping_data *data, int exit_code);

// Create the raw socket
void create_socket(ping_data *data);

// Create the ICMP packet
void build_icmp_packet(ping_data *data);

// Send and receive ICMP packets
int send_ping(ping_data *data, struct timeval current_time);
void recv_ping(ping_data *data, struct timeval current_time);

#endif