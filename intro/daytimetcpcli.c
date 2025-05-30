#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 4096 /* max text line length */
#define SA struct sockaddr

void err_sys(const char *x) {
  perror(x);
  exit(1);
}

int main(int argc, char **argv) {
  int socket_fd, n;
  struct sockaddr_in serveraddr;
  char recvline[MAXLINE + 1];

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <IP address>\n", argv[0]);
    exit(1);
  }

  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    err_sys("socket error");
  }

  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(13);

  if ((inet_pton(AF_INET, argv[1], &serveraddr.sin_addr)) < 0) {
    err_sys("inet_pton error");
  }

  int r = connect(socket_fd, (SA *)&serveraddr, sizeof(serveraddr));

  if (r < 0) {
    err_sys("connect error");
  }

  while((n = read(socket_fd, recvline, MAXLINE)) > 0) {
    recvline[n] = 0;
    if (fputs(recvline, stdout) == EOF) {
      err_sys("fputs error");
    }
  }

  if (n < 0) {
    err_sys("read error");
  }

  return 0;
}
