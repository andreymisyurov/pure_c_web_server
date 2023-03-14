#ifndef _SRC_STRUCT_H_
#define _SRC_STRUCT_H_

#include <pthread.h>

typedef struct pair {
  int m_cl_sock;
  char m_path[2048];
  pthread_mutex_t m_mutex;
} pair;

#endif  // _SRC_STRUCT_H_