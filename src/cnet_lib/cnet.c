#include "cnet.h"

int get_socket(int port, int max_conn) {
  int server_socket = create_socket();
  struct sockaddr_in server_address;
  set_options(port, &server_address);
  if (bind_socket(server_socket, &server_address) < 0) return 1;
  listen(server_socket, max_conn);
  return server_socket;
}

int create_socket() {
  int result = socket(AF_INET, SOCK_STREAM, 0);
  if (result == -1) {
    printf("Error creating socket");
    return -1;
  }
  return result;
}

void set_options(int port, struct sockaddr_in *server_addr) {
  memset(server_addr, 0, sizeof(*server_addr));
  server_addr->sin_family = AF_INET;
  server_addr->sin_addr.s_addr = INADDR_ANY;
  server_addr->sin_port = htons(port);
}

int bind_socket(int server_socket, struct sockaddr_in *server_address) {
  if (bind(server_socket, (struct sockaddr *)server_address,
           sizeof(*server_address)) < 0) {
    printf("Error binding");
    //    return -1;
    exit(1);
  }
  return 0;
}

void listen_http(int server_socket, char *path, func_ptr send_file) {
  printf("Waiting for incoming connections...\n");

  int client_socket = accept(server_socket, NULL, NULL);
  while (client_socket != 0) {
    printf("Connection accepted\n");
    //    send_file(client_socket, path);
    close(client_socket);
    client_socket = accept(server_socket, NULL, NULL);
  }

  if (client_socket < 0) {
    printf("Error accepting connection");
    exit(1);
  }
}
