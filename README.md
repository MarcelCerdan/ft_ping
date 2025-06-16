# ft_ping
Recoding the ping command using the inetutils-2.0 implementation as a reference.

TO DO LIST :

	1- OK - If the user input a hostname, resolve it to an IP adress (using a DNS server)

	2- OK - Create a raw socket and configure it to use ICMP protocol (IPPROTO_ICMP)

	3- OK - Build the ICMP packet "Echo request" :	-> Type: 8
												-> Code: 0
												-> Checksum
												-> Identifier: often th ID of the ping process
												-> Sequence number: a sequence number that increment at each paquet send
												-> Data: Optional payload. Often contains the time to calculate the round-trip (RTT).
	
	4- OK - Send the paquet using a function like sendto()

	5- OK - Wait for the "Echo reply" ICMP paquet :	-> Ping wait for an answer using a function like recvfrom()
												-> It uses a timeout, if no answer is received in a given time, the paquet is considered lost.
	
	6- OK - Reception and processing of the "Echo reply" paquet :	-> Ping checks if it's a ICMP paquet
																-> It checks if it's a "Echo reply" (type 0)
																-> It checks the identifier
																-> Examines the sequence number
																-> Validates the checksum
		if everything is correct, calculates the RTT and print informations like the source IP, the sequence number and the RTT.
	
	7- Errors and ICMP messages :	-> Ping can receive other message in answer : Destination unreachable and Time exceed (TTL = 0)

	8- Loops and stats :	-> Default ping send paquets every 1 second until the user CTRL+C
							-> At the end of the execution, ping prints stats resuming :	+ The number of paquets transmited
																							+ The number of paquets received
																							+ The percentage of paquets lost
																							+ The min, max and average RTT time (eventually standard deviation)

The PING struct used in inetutils-2.0 :

typedef struct ping_data PING;

			struct ping_data
			{
				int ping_fd;                 /* Raw socket descriptor */
				int ping_type;               /* Type of packets to send */
				size_t ping_count;           /* Number of packets to send */
				struct timeval ping_start_time; /* Start time */
				size_t ping_interval;        /* Number of seconds to wait between sending pkts */
				union ping_address ping_dest;/* whom to ping */
				char *ping_hostname;         /* Printable hostname */
				size_t ping_datalen;         /* Length of data */
				int ping_ident;              /* Our identifier */
				union event ping_event;      /* User-defined handler */
				void *ping_closure;          /* User-defined data */

				/* Runtime info */
				int ping_cktab_size;
				char *ping_cktab;

				unsigned char *ping_buffer;         /* I/O buffer */
				union ping_address ping_from;
				size_t ping_num_xmit;        /* Number of packets transmitted */
				size_t ping_num_recv;        /* Number of packets received */
				size_t ping_num_rept;        /* Number of duplicates received */
			};
