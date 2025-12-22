#include "ft_ping.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int check_args(char **av, ping_data *data) {
	if (av[0][0] == '-') {
		return parse_options(av, data);
	}
	else {
		check_address(*av, data);
		return -1;
	}
}

int parse_options(char **av, ping_data *data) {
    int i = 1;
    int consumed = 0;
	char *val_str;

    while (av[0][i] != '\0') {
        char key = av[0][i++];
        switch (key) {
			case 'h':
			case '?':
				printf("Usage: ft_ping [options] <hostname>\n");
				printf("Options:\n");
				printf("  -h\t\tDisplay this help message\n");
				printf("  -c <count>\tStop after sending <count> packets\n");
				printf("  -i <interval>\tWait <interval> seconds between sending packets\n");
				exit(0);
            case 'v':
                data->ping_verbose = 1;
                break;
            case 'c':
            case 'i':
			case 'W':
				if (av[0][i] != '\0') {
					val_str = &av[0][i];
					i = strlen(av[0]); // Move to end of string
				}
				else {
					av++;
					if (!*av)
						error_msg("Option requires an argument", data);
					val_str = *av;
					consumed++;
				}

				if (!is_number(val_str)) {
					char msg[64];
					snprintf(msg, sizeof(msg), "Invalid argument for -%c option. Must be a positive number.", key);
					error_msg(msg, data);
				}

				long val = strtoul(val_str, NULL, 10);
				if (key == 'c')
					data->ping_count = val;
				else if (key == 'i')
					data->ping_interval = val;
				else if (key == 'W')
					data->timeout = val;
				
				return consumed; // Consumed option and its argument
            case 'n':
				data->opt_numeric = 1;
                break;
			case '-':
			if (av[1])
				consumed += handle_long_option(&av[0][i], av[1], data);
			else
				consumed += handle_long_option(&av[0][i], NULL, data);
			return consumed; // Consumed the long option
			default:
				fprintf(stderr, "Invalid option: -%c\n", key);
				fprintf(stderr, "Use -h for usage information.\n");
				exit(1);
        }
    }
    return consumed;
}

int handle_long_option(char *option, char *value, ping_data *data) {
	if (strcmp(option, "count") == 0) {
		if (!is_number(value)) {
			error_msg("Invalid argument for --count option. Must be a number.", data);
		}
		data->ping_count = strtoul(value, NULL, 10);
		return 1;
	} else if (strcmp(option, "interval") == 0) {
		if (!is_number(value)) {
			error_msg("Invalid argument for --interval option. Must be a number.", data);
		}
		data->ping_interval = strtoul(value, NULL, 10);
		return 1;
	} else if (strcmp(option, "verbose") == 0) {
		data->ping_verbose = 1;
		return 0;
	} else if (strcmp(option, "numeric") == 0) {
		data->opt_numeric = 1;
		return 0;
	} else if (strcmp(option, "ttl") == 0) {
		if (!is_number(value)) {
			error_msg("Invalid argument for --ttl option. Must be a number.", data);
		}
		data->ttl_val = strtoul(value, NULL, 10);
		return 1;
	}
	else {
		fprintf(stderr, "Invalid option: --%s\n", option);
		fprintf(stderr, "Use -h or -? for usage information.\n");
		exit(1);
	}
}

void check_address(char *hostname, ping_data *data) {
	struct addrinfo hints, *res, *p;
	int status;

	memset(&hints, 0, sizeof hints); // Be sure the struct is empty
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_RAW; // Ping uses a raw socket

	if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo for '%s' failed : %s \n", hostname, gai_strerror(status));
		exit_clean(NULL, data, 1);
	}
	
	if (res == NULL) {
    	fprintf(stderr, "No address found for '%s'\n", hostname);
    	exit_clean(NULL, data, 1);
	}

	for (p = res; p != NULL; p = p->ai_next) {
		void *addr;

		// Get the pointer to the address itself
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
		addr = &(ipv4->sin_addr);

		// Convert the IP to a string and print it
		if (inet_ntop(p->ai_family, addr, data->ip_str, sizeof data->ip_str) != NULL) {
			data->ping_hostname = strdup(hostname);
			check_malloc("ping_hostname", data->ping_hostname, data);
			data->dest_addr = *ipv4; // Store the destination address
		}
		else {
			fprintf(stderr, "inet_ntop failed: %s\n", strerror(errno));
			freeaddrinfo(res);
			exit_clean(data, NULL, 1);
		}
	}

	freeaddrinfo(res);
}

