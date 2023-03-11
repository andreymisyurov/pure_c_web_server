#include "cnet.h"

int get_socket(int in_port, int in_max_conn) {
  int server_sock = create_socket();
  if(server_sock < 0) return -1;
  struct sockaddr_in server_address = {0};
  set_options(in_port, &server_address);
  if(bind_socket(server_sock, &server_address) < 0) return -1;
  if(listen(server_sock, in_max_conn) < 0) {
    perror("Error listening");
    close(server_sock);
    return -1;
  };
  return server_sock;
}

int create_socket() {
  int result = socket(AF_INET, SOCK_STREAM, 0);
  int yes = 1;
  if(setsockopt(result, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("Error setting socket options");
    close(result);
    result = -1;
  }
  return result;
}

void set_options(int in_port, struct sockaddr_in *out_server_addr) {
  memset(out_server_addr, 0, sizeof(*out_server_addr));
  out_server_addr->sin_family = AF_INET;
  out_server_addr->sin_addr.s_addr = INADDR_ANY;
  out_server_addr->sin_port = htons(in_port);
}

int bind_socket(int in_server_sock, struct sockaddr_in *in_server_add) {
  int reuse = 1;
  if(setsockopt(in_server_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
    perror("Error bind_socket()");
    return -1;
  }
  int bind_result = bind(in_server_sock, (struct sockaddr *)in_server_add, sizeof(*in_server_add));
  while (bind_result < 0 && errno == EADDRINUSE) {
    sleep(1);
    bind_result = bind(in_server_sock, (struct sockaddr *)in_server_add, sizeof(*in_server_add));
  }

  if(bind_result < 0) {
    printf("Error binding");
    return -1;
  }
  return 0;
}

void listen_http(int in_server_sock, char *in_path, func_ptr in_send_file_foo) {
  printf("Waiting for incoming connections...\n");

  int client_socket = -1;
  while(1) {
    client_socket = accept(in_server_sock, NULL, NULL);
    if(client_socket < 0) {
      perror("Error accepting connection");
      break;
    }

    printf("Connection accepted\n");
    if(in_send_file_foo(client_socket, in_path) == 0) {
      perror("Operation completed successfully");
    } else {
      perror("Couldn't complete operation");
    }
    if(close(client_socket) < 0) {
      perror("Error closing client socket");
    }
  }
}
