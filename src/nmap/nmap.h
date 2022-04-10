#include "unp.h"
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include <ifaddrs.h>
#include <netinet/if_ether.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <string.h>
#include <linux/if_packet.h>

#define BUFSIZE 1500

extern char sendbuf[BUFSIZE];

extern int datalen; /* # bytes of data following ICMP header */
extern char *host;
extern int nsent; /* add 1 for each sendto() */
extern pid_t pid; /* our PID */
extern int sockfd;
extern int verbose;


/* function prototypes */
void proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
void send_v4(void);
void readloop(void);
void sig_alrm(int);
void tv_sub(struct timeval *, struct timeval *);
void write_file(char* fileName, char* buff, char* mode);

struct proto {
  void (*fproc)(char *, ssize_t, struct msghdr *, struct timeval *);
  void (*fsend)(void);
  void (*finit)(void);
  void (*fwrite_file)(char* fileName, char* buff, char* mode);
  struct sockaddr *sasend; /* sockaddr{} for send, from getaddrinfo */
  struct sockaddr *sarecv; /* sockaddr{} for receiving */
  socklen_t salen;         /* length of sockaddr{}s */
};

struct store {
  uint8_t req_sha[ETH_ALEN];	/* sender hardware (MAC) address */
  uint8_t req_spa[4];		/* sender protocol (IP) address */
  char* file_name;
  int ip_index;
  int host_up;
};

extern struct proto *pr;
extern struct store *store;
