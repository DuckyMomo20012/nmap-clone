#include "ping.h"

char sendbuf[BUFSIZE];

int datalen; /* # bytes of data following ICMP header */
char *host;
int nsent; /* add 1 for each sendto() */
pid_t pid; /* our PID */
int sockfd;
int verbose;

struct proto *pr;

// struct proto proto_v4 = {proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_IP};
struct proto proto_v4 = {proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_IP};

int datalen = 28; /* default arp packet size */

int main(int argc, char **argv) {
  int c;
  struct addrinfo *ai;
  char *h;

  opterr = 0; /* don't want getopt() writing to stderr */
  while ((c = getopt(argc, argv, "v")) != -1) {
    switch (c) {
    case 'v':
      verbose++;
      break;

    case '?':
      err_quit("unrecognized option: %c", c);
    }
  }

  if (optind != argc - 1)
    err_quit("usage: ping [ -v ] <hostname>");
  host = argv[optind];

  pid = getpid() & 0xffff; /* ICMP ID field is 16 bits */



  ai = Host_serv(host, NULL, 0, 0);

  h = Sock_ntop_host(ai->ai_addr, ai->ai_addrlen);
  printf("PING %s (%s): %d data bytes\n",
         ai->ai_canonname ? ai->ai_canonname : h, h, datalen);

  /* 4initialize according to protocol */
  if (ai->ai_family == AF_INET) {
    pr = &proto_v4;
  } else
    err_quit("unknown address family %d", ai->ai_family);

  // Set send ARP request from .1 -> .254
  pr->ip_index = 1;

  Signal(SIGALRM, sig_alrm);

  pr->sasend = ai->ai_addr;
  pr->sarecv = Calloc(1, ai->ai_addrlen);
  pr->salen = ai->ai_addrlen;

  readloop();

  exit(0);
}