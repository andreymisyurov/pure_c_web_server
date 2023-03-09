#include "./cnet_lib/cnet.h"
#include "./http_lib/http.h"

#define MAX_CONNECTIONS 10

int main(int argc, char *argv[]) {
  int port = 8080;
  char *path = ".";

  int server_socket = get_socket(port, MAX_CONNECTIONS);

  listen_http(server_socket, path, send_response);

  return 0;
}
