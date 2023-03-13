#include "http.h"

#ifdef MULTI_TH
void send_response(pair *in_pair) {
  int cl_sock = in_pair->cl_sock;
  char buff[BUFFER_SIZE];
  memset(buff, 0, BUFFER_SIZE);
  strcpy(buff, in_pair->path);

  char *client_message = calloc(BUFFER_SIZE, sizeof(char));
  if (!client_message) {
    close(cl_sock);
    return;
  }
  int read_size = (int)recv(cl_sock, client_message, BUFFER_SIZE, 0);
  if (read_size <= 0) {
    free(client_message);
    perror("Error in recv");
    close(cl_sock);
    return;
  }

  HTTP_answer response_info = parse_response(client_message);
  free(client_message);
  if (check_response_info(cl_sock, response_info.file) != 0) {
    close(cl_sock);
    return;
  }

  char *full_path = calloc(BUFFER_SIZE, sizeof(char));
  if (!full_path) {
    close(cl_sock);
    return;
  }
  sprintf(full_path, "%s%s", buff, response_info.file);

  if (!strcmp(response_info.type, "DELETE")) {
    remove_html(cl_sock, full_path);
    free(full_path);
    close(cl_sock);
    return;
  }

  if (check_file(full_path)) {
    free(full_path);
    perror("error in full_path: ");
    send_error_404(cl_sock);
    close(cl_sock);
    return;
  }

  send_file(in_pair->cl_sock, full_path);
  free(full_path);
  close(in_pair->cl_sock);
  sleep(1);
}

#else

int send_response(int in_client_sock, char *in_path) {
  char *client_message = calloc(BUFFER_SIZE, sizeof(char));
  if (!client_message) return -1;
  int read_size = (int)recv(in_client_sock, client_message, BUFFER_SIZE, 0);
  if (read_size <= 0) {
    free(client_message);
    perror("Error in recv");
    return -2;
  }
  HTTP_answer response_info = parse_response(client_message);
  free(client_message);
  if (check_response_info(in_client_sock, response_info.file) != 0) return -7;

  char *full_path = calloc(BUFFER_SIZE, sizeof(char));
  if (!full_path) return -3;
  sprintf(full_path, "%s%s", in_path, response_info.file);

  if (!strcmp(response_info.type, "DELETE")) {
    remove_html(in_client_sock, full_path);
    free(full_path);
    return -5;
  }

  if (check_file(full_path)) {
    free(full_path);
    perror("error in full_path: ");
    send_error_404(in_client_sock);
    return -4;
  }

  int result = send_file(in_client_sock, full_path);
  free(full_path);
  sleep(1);
  return result;
}

#endif  // MULTI_TH

int send_file(int in_client_sock, char *in_file_path) {
  if (!in_file_path || !strlen(in_file_path)) return -8;
  char *result = create_answer(in_file_path);
  if (!result) return -6;

  int check_send = send(in_client_sock, result, strlen(result), 0);
  free(result);
  if (check_send < 0) perror("Error in send");
  return 0;
}

void send_error_404(int in_client_sock) {
  char fail[78] =
      "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<h1>404 Page Not "
      "Found</h1>";
  send(in_client_sock, fail, strlen(fail), 0);
}

int check_response_info(int in_sock, char *in_message) {
  if (!in_message || strlen(in_message) > BUFFER_SIZE || !strlen(in_message)) {
    send_error_404(in_sock);
    return -1;
  }
  //  if(!strcmp(in_message, "/favicon.ico")) return -1;
  return 0;
}

int check_file(char *in_path) {
  if (!in_path) return -2;
  regex_t regex;
  int reti = regcomp(&regex, "\\.html$", REG_ICASE);
  if (reti) {
    regfree(&regex);
    return -1;
  }

  reti = regexec(&regex, in_path, 0, NULL, 0);
  regfree(&regex);
  if (reti) {
    return 1;
  }

  if (access(in_path, F_OK) == -1) {
    return 2;
  }
  return 0;
}

char *create_answer(char *in_file_path) {
  // func allocates memory by calloc() and returns a string
  if (!in_file_path || !strlen(in_file_path)) return NULL;
  FILE *file = fopen(in_file_path, "r");
  if (!file) {
    fclose(file);
    return NULL;
  }
  char *result = calloc(BUFFER_SIZE * 10, sizeof(char));
  if (!result) return NULL;

  strcat(result, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));
  if (!buffer) {
    free(result);
    return NULL;
  }

  while (fgets(buffer, BUFFER_SIZE, file)) {
    strcat(result, buffer);
    memset(buffer, 0, BUFFER_SIZE);
  }

  free(buffer);
  fclose(file);
  return result;
}

void remove_html(int client_socket, char *in_file_path) {
  char html_fail[71] =
      "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<h1>Delete FAIL</h1>";
  char html_success[67] =
      "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<h1>Delete SUCCESS</h1>";
  int check_rm = remove(in_file_path);
  if (check_rm == 0) {
    send(client_socket, html_success, strlen(html_success), 0);
  } else {
    send(client_socket, html_fail, strlen(html_fail), 0);
  }
}

HTTP_answer parse_response(char *in_str) {
  HTTP_answer result = {0};
  if (!in_str || strlen(in_str) > BUFFER_SIZE) return result;
  char temp_type[BUFFER_SIZE];
  char temp_file[BUFFER_SIZE];
  char temp_proto[BUFFER_SIZE];
  int count = sscanf(in_str, "%s %s %s", temp_type, temp_file, temp_proto);
  if (count == 3 && strlen(temp_type) < 8 && strlen(temp_proto) < 16) {
    strcpy(result.type, temp_type);
    strcpy(result.file, temp_file);
    strcpy(result.proto, temp_proto);
  }
  return result;
}
