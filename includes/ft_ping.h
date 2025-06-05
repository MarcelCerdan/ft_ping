#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <sys/time.h>
# include <sys/types.h>
# include <sys/socket.h>

# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/in.h>

struct ping_data
{
	int ping_fd;					/* Raw socket */
	int ping_type;					/* Type of packets to send */
	int ping_id;					/* Ping identifier */
	size_t ping_count;				/* Number of packets to send */
	size_t ping_interval;			/* Number of second to wait between packets */
	struct timeval ping_start_time;	/* Start time */
	char *ping_hostname;			/* Printable hostname */
	char *ping_buffer;
};

int check_args(char *arg);
int parse_options(int key);
int check_address(char *address);

#endif