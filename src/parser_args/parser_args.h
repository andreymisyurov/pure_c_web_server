#include <sys/stat.h>

int isDirectoryExists(const char *path) {
  struct stat stats;
  stat(path, &stats);
  if(S_ISDIR(stats.st_mode))
    return 1;
  return 0;
}

const char *argp_program_version = "0.1";
const char *argp_program_bug_address = "andrey.misyrov@gmail.com";
static char doc[] = "Web-server was written on pure C";
static char args_doc[] = "";
static struct argp_option options[] = {{"port",      'p', "PORT", 0, "choose a port for listen"},
                                       {"directory", 'd', "DIR",  0, "type a directory for html files"},
                                       {0}};

struct arguments {
  int port;
  char dir[BUFFER_SIZE];
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch(key) {
    case 'p': {
      int value = atoi(arg);
      if(value <= 0) {
        printf("Port was entered incorrectly\n");
        exit(-1);
      }
      arguments->port = value;
      break;
    }
    case 'd': {
      if(!isDirectoryExists(arg)) {
        printf("This directory is not exist: \"%s\"\n", arg);
        exit(-1);
      }
      strcpy(arguments->dir, arg);
      if(arg[strlen(arg) - 1] != '/') {
        strcat(arguments->dir, "/");
      }
      break;
    }
    default:return ARGP_ERR_UNKNOWN;
  }
  return 0;
}
static struct argp argp = {options, parse_opt, args_doc, doc};
