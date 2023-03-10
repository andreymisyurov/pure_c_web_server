#include "cnet.h"

int get_socket(int port, int max_conn) {
  int server_socket = create_socket();
  if(server_socket < 0) return -1;
  struct sockaddr_in server_address;
  set_options(port, &server_address);
  if(bind_socket(server_socket, &server_address) < 0) return -1;
  if(listen(server_socket, max_conn) < 0) return -1;
  return server_socket;
}

int create_socket() {
  int result = socket(AF_INET, SOCK_STREAM, 0);
  if(result == -1) {
    perror("Error creating socket");
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
  if(bind(server_socket, (struct sockaddr *)server_address, sizeof(*server_address)) < 0) {
    printf("Error binding");
    return -1;
  }
  return 0;
}

void listen_http(int server_socket, char *path, func_ptr send_file_func) {
  printf("Waiting for incoming connections...\n");

  int client_socket;
  while(1) {
    client_socket = accept(server_socket, NULL, NULL);
    if(client_socket < 0) {
      perror("Error accepting connection");
      break;
    }

    printf("Connection accepted\n");
    if(send_file_func(client_socket, path) == 0) {
      perror("Operation was completed successfully");
    } else {
      perror("Couldn't complete operation");
    }
    if(close(client_socket) < 0) {
      perror("Error closing client socket");
    }
  }
}
