#include "nmap.h"

char sendbuf[BUFSIZE];

int datalen; /* # bytes of data following ICMP header */
char *host;
int nsent; /* add 1 for each sendto() */
pid_t pid; /* our PID */
int sockfd;
int verbose;

struct proto *pr;

// struct proto proto_v4 = {proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_IP};
struct proto proto_v4 = {proc_v4, send_v4, NULL, write_file, NULL, NULL, 0};


struct store *store;

// Set send ARP request from .1 -> .254
struct store store_arp = {"", "", "19127631.txt", 1, 0, 5.0};



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
    err_quit("usage: nmap [ -v ] <hostname>");
  host = argv[optind];

  int ip[4];

  // Remove subnet mask if needed. e.g: 192.168.202.128/24 -> 192.168.202.128
  // REVIEW: Should handle this simpler
  sscanf(host, "%i.%i.%i.%i", &ip[0], &ip[1], &ip[2], &ip[3]);
  sprintf(host, "%i.%i.%i.%i", ip[0], ip[1], ip[2], ip[3]);

  pid = getpid() & 0xffff; /* ICMP ID field is 16 bits */

  ai = Host_serv(host, NULL, 0, 0);

  h = Sock_ntop_host(ai->ai_addr, ai->ai_addrlen);
//   printf("PING %s (%s): %d data bytes\n",
//          ai->ai_canonname ? ai->ai_canonname : h, h, datalen);

  /* 4initialize according to protocol */
  if (ai->ai_family == AF_INET) {
    pr = &proto_v4;
  } else
    err_quit("unknown address family %d", ai->ai_family);


  store = &store_arp;

  (*pr->fwrite_file)(store->file_name, "\0", "w"); // Clear file

  Signal(SIGALRM, sig_alrm); // Register signal handler

  pr->sasend = ai->ai_addr;
  pr->sarecv = Calloc(1, ai->ai_addrlen);
  pr->salen = ai->ai_addrlen;

  readloop();

  exit(0);
}
