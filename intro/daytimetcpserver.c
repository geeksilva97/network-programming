#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define MAXLINE 4096 /* max text line length */
/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define LISTENQ 1024 /* 2nd argument to listen() */
#define SA struct sockaddr

void err_sys(const char *x) {
  perror(x);
  exit(1);
}

int main() {
  int listenfd, connfd;
  struct sockaddr_in serveraddr;
  time_t ticks;
  char buff[MAXLINE];

  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;

  // https://man7.org/linux/man-pages/man7/ip.7.html
  /* Special and reserved addresses */
  /*       There are several special addresses: */

  /*       INADDR_LOOPBACK (127.0.0.1) */
  /*              always refers to the local host via the loopback device; */

  /*       INADDR_ANY (0.0.0.0) */
  /*              means any address for socket binding; */

  /*       INADDR_BROADCAST (255.255.255.255) */
  /*              has the same effect on bind(2) as INADDR_ANY for historical */
  /*              reasons.  A packet addressed to INADDR_BROADCAST through a */
  /*              socket which has SO_BROADCAST set will be broadcast to all */
  /*              hosts on the local network segment, as long as the link is */
  /*              broadcast-capable. */
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(13);

  if ((bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr))) < 0){
    err_sys("Bind error");
  }

  if ((listen(listenfd, LISTENQ)) < 0) {
    err_sys("Listen error");
  }

  printf("Daytime server listening on port 13...\n");

  for (;;) {
    connfd = accept(listenfd, (SA *)NULL, NULL);
    printf("New connection accepted\n");
    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    write(connfd, buff, strlen(buff));
    printf("Sent time: %.24s\r\n", ctime(&ticks));
    close(connfd);
    printf("Connection closed\n");
  }

  return 0;
}
