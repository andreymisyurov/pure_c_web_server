#ifndef _TYPE_HTTP_
#define _TYPE_HTTP_

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
void send_response(int client_socket, char *path);

#endif  // _TYPE_HTTP_