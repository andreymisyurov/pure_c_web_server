#include "http.h"

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

int check_file(char *fullpath) {
  if (!fullpath) return -2;
  regex_t regex;
  int reti = regcomp(&regex, "\\.html$", REG_ICASE);
  if (reti) {
    regfree(&regex);
    return -1;
  }

  reti = regexec(&regex, fullpath, 0, NULL, 0);
  regfree(&regex);
  if (reti) {
    return 1;
  }

  if (access(fullpath, F_OK) == -1) {
    return 2;
  }
  return 0;
}

char *create_answer(char *full_path) {
  FILE *file = fopen(full_path, "r");
  if (!file) {
    fclose(file);
    return NULL;
  }
  char *result = calloc(BUFFER_SIZE * 10, sizeof(char));
  strcat(result, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");

  if (!result) return NULL;
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

void remove_html(int client_socket, char *full_path) {
  char fail[73] =
      "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\nCouldn't delete this "
      "page";
  char success[71] =
      "HTTP/1.1 200 OK\nContent-Type: text/html\n\nThis page deleted "
      "successfully";
  int check_rm = remove(full_path);
  if (check_rm == 0) {
    send(client_socket, success, strlen(success), 0);
  } else {
    send(client_socket, fail, strlen(fail), 0);
  }
}

void send_response(int client_socket, char *path) {
  char *client_message = calloc(BUFFER_SIZE, sizeof(char));
  if (!client_message) return;

  int read_size = recv(client_socket, client_message, BUFFER_SIZE, 0);
  if (read_size <= 0) {
    free(client_message);
    perror("Error in recv");
    return;
  }

  HTTP_answer response_info = parse_response(client_message);
  free(client_message);
  if (!strcmp(response_info.file, "/favicon.ico")) return;

  char *full_path = calloc(BUFFER_SIZE, sizeof(char));
  if (!full_path) return;
  sprintf(full_path, "%s%s", path, response_info.file);

  if (check_file(full_path)) {
    free(full_path);
    perror("error in full_path: ");
    return;
  }

  if (!strcmp(response_info.type, "DELETE")) {
    remove_html(client_socket, full_path);
    free(full_path);
    return;
  }

  char *result = create_answer(full_path);
  free(full_path);
  if (!result) return;

  int check_send = send(client_socket, result, strlen(result), 0);
  free(result);

  if (check_send < 0) perror("Error in send");
}