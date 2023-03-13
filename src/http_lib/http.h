#ifndef _HTTP_LIB_HTTP_
#define _HTTP_LIB_HTTP_

#include <arpa/inet.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef MULTI_TH
#include "./../struct.h"
#endif  // MULTI_TH

#define BUFFER_SIZE 2048

typedef struct HTTP_answer {
  char type[8];
  char file[BUFFER_SIZE];
  char proto[16];
} HTTP_answer;

#ifdef MULTI_TH
void send_response(pair *in_pair);
#else
int send_response(int client_socket, char *path);
#endif  // MULTI_TH

int send_file(int in_client_sock, char *in_file_path);
int check_response_info(int in_sock, char *in_message);
int check_file(char *fullpath);
char *create_answer(char *full_path);
void remove_html(int in_client_socket, char *full_path);
void send_error_404(int in_client_socket);
HTTP_answer parse_response(char *in_str);

#endif  // _HTTP_LIB_HTTP_