#include "ft_ping.h"

void print_statistics(ping_data *data) {
    double avg = data->sum_rtt / data->packets_received;
    double stddev = (data->sum_rtt_squared / data->packets_received) - (avg * avg);

    printf("--- %s ping statistics ---\n", data->ping_hostname);
    printf("%ld packets transmitted, %ld packets received, %.1f%% packet loss\n",
           data->packets_sent, data->packets_received,
           ((data->packets_sent - data->packets_received) / (double)data->packets_sent) * 100.0);
    if (data->packets_received > 0) {
        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
               data->min_rtt, avg, data->max_rtt, stddev);
    }
}