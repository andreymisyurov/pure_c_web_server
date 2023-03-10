#ifndef _CNET_LIB_CNET_H_
#define _CNET_LIB_CNET_H_

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef int (*func_ptr)(int, char *);

int get_socket(int in_port, int in_max_conn);

int create_socket();
void set_options(int in_port, struct sockaddr_in *out_server_addr);
int bind_socket(int in_server_sock, struct sockaddr_in *in_server_add);
void listen_http(int in_server_sock, char *in_path, func_ptr in_send_file_foo);

#endif  // _CNET_LIB_CNET_H_