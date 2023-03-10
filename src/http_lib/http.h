#ifndef _HTTP_LIB_HTTP_
#define _HTTP_LIB_HTTP_

#include <arpa/inet.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 2048

typedef struct HTTP_answer {
  char type[8];  // буффер для запросов PUT, GET, POST, DELETE
  char file[BUFFER_SIZE];
  char proto[16];
} HTTP_answer;

HTTP_answer parse_response(char *in_str);
int check_file(char *fullpath);
char *create_answer(char *full_path);
void remove_html(int client_socket, char *full_path);
int send_response(int client_socket, char *path);
void send_error_404(int client_socket);

#endif  // _HTTP_LIB_HTTP_