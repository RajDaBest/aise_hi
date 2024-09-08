#ifndef __NETWORK_UTILS__
#define __NETWORK_UTILS__

#include <stdint.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <memory.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>

#define MAX_PACKET_BUFFER_SIZE 1024

typedef void (*recv_fn_cb)(char *, uint32_t, char *, uint32_t); // msg received, received msg size, sender's IP address, sender's port number

pthread_t *
udp_server_create_and_start(char *ip_addr, uint32_t udp_port_no, recv_fn_cb recv_fn);

int 
send_udp_msg(char *dest_ip_buffer, uint32_t udp_port_no, char *msg, uint32_t msg_size);

// General network utilities

char*
network_covert_ip_n_to_p (uint32_t ip_addr, char* output_buffer);

uint32_t
network_covert_ip_p_to_n (char* ip_addr);

#endif