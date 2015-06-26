
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <rdma/rsocket.h>
#define RECVBUFFSIZE 1<<30

double
stopwatch (void)
{
  double diff = 0.0;

  struct timeval timenow;
  static struct timeval timeprev;	/* static to save prev. time */

  gettimeofday (&timenow, NULL);

  diff =
    ((double) timenow.tv_sec + 1.0e-6 * (double) timenow.tv_usec) -
    ((double) timeprev.tv_sec + 1.0e-6 * (double) timeprev.tv_usec);

  timeprev.tv_sec = timenow.tv_sec;
  timeprev.tv_usec = timenow.tv_usec;

  return diff;
}

int
main (int argc, char *argv[])
{
  int sockfd = 0;
  ssize_t n = 0, bytes = 0;
  char *recvBuff = NULL;
  struct sockaddr_in serv_addr;
  double time;

  if (argc != 2)
    {
      printf ("Usage: %s <ip of server>\n", argv[0]);
      return 1;
    }

  recvBuff = calloc (1, RECVBUFFSIZE);

  if ((sockfd = rsocket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
      printf ("ERROR: socket() failed\n");
      return 1;
    }

  memset (&serv_addr, '0', sizeof (serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons (1337);

  if (inet_pton (AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
    {
      printf ("ERROR: inet_pton() failed\n");
      return 1;
    }

  if (connect (sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) <
      0)
    {
      printf ("ERROR: connect() failed\n");
      return 1;
    }

  time = stopwatch ();

  while ((n = read (sockfd, recvBuff, RECVBUFFSIZE)) > 0)
    {
      bytes += n;
      continue;
    }

  time = stopwatch ();

  printf
    ("read() bandwidth =\n\t %f bytes/s\n\t %f mebibytes/s\n\t %f gigabit/s\n",
     bytes / time, bytes / (1 << 20) / time, bytes * 8 / (1 << 30) / time);

  if (n < 0)
    {
      printf ("ERROR: read() failed\n");
    }

  return 0;
}
