#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define SENDBUFFSIZE 1<<30

int
main (int argc, char *argv[])
{
  int listenfd = 0, connfd = 0;
  struct sockaddr_in serv_addr;

  char *sendBuff = NULL;

  listenfd = socket (AF_INET, SOCK_STREAM, 0);

  int on = 1;

  if (setsockopt (listenfd, SOL_SOCKET,
		  SO_REUSEADDR, (const char *) &on, sizeof (on)))
    {
      printf ("ERROR: setsockopt() failed\n");
    }

  memset (&serv_addr, '0', sizeof (serv_addr));

  sendBuff = calloc (1, SENDBUFFSIZE);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
  serv_addr.sin_port = htons (1337);

  bind (listenfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr));

  listen (listenfd, 10);

  while (1)
    {
      connfd = accept (listenfd, (struct sockaddr *) NULL, NULL);

      write (connfd, sendBuff, SENDBUFFSIZE);

      close (connfd);
      sleep (1);
    }
}
