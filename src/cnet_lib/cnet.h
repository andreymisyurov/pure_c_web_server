#ifndef _EX_NET_H_
#define _EX_NET_H_

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef void (*func_ptr)(int, char *);

int get_socket(int port, int max_conn);

int create_socket();
void set_options(int port, struct sockaddr_in *server_addr);
int bind_socket(int server_socket, struct sockaddr_in *server_address);
void listen_http(int server_socket, char *path, func_ptr send_file);

#endif  // _EX_NET_H_