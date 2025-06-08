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

typedef struct ping_data
{
	int ping_fd;					/* Raw socket */
	int ping_type;					/* Type of packets to send */
	int ping_id;					/* Ping identifier */
	size_t ping_count;				/* Number of packets to send */
	size_t ping_interval;			/* Number of second to wait between packets */
	struct timeval ping_start_time;	/* Start time */
	char *ping_hostname;			/* Printable hostname */
	char *ping_buffer;
} ping_data;

// --- FUNCTIONS PROTOTYPES --- //

// Parse and check arguments
void check_args(char *arg, ping_data *data);
void parse_options(int key);
void check_address(char *address, ping_data *data);

// Handling errors, memory and exit
void clean_ping_data(ping_data *data);
void check_malloc(char *error_msg, void *addr);
void exit_clean(void *data_to_free, ping_data *data, int exit_code);

// Create the raw socket
void create_socket(ping_data *data);

#endif