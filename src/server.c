#include "./cnet_lib/cnet.h"
#include "./http_lib/http.h"
#include "./parser_args/parser_args.h"

#define MAX_CONNECTIONS 10

int main(int argc, char *argv[]) {
  struct arguments arguments = {8085, "./src/templates/"};
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  printf("Try to start server on port: %d\nRoot Directory: %s\n", arguments.port, arguments.dir);
  int server_socket = get_socket(arguments.port, MAX_CONNECTIONS);
  if(server_socket > 0) listen_http(server_socket, arguments.dir, send_response);
  return 0;
}
