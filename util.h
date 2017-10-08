#include <string.h>
#include <strings.h>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <iomanip>
#include <errno.h>
#include <signal.h>

#define MSG_SIZE 1024

using namespace std;

void error(const char *msg){
    perror(msg);
    exit(1);
}

int send_msg(int socket, char *msg) {
  int ret;
  while (1) {
    ret=send(socket, msg, MSG_SIZE, MSG_NOSIGNAL);
    if (ret==-1 && errno == EINTR) continue;
    else if (ret == -1 && errno == EPIPE) return -1;
    else break;
  }
  return ret;
}

size_t recv_msg(int socket, char *buff, size_t buf_len) {
  int ret;
  while (1) {
    memset(buff, 0, buf_len);
    ret=recv(socket, buff, buf_len, 0 );
    if (ret==-1 && errno==EINTR) continue;
    else if (ret==-1 && errno==EWOULDBLOCK ) {
      return -1;
    }
    else if(ret==0){
      return -2;
    }
    else break;
  }
  return ret;
}
