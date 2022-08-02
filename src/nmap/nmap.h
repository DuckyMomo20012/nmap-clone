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
  // REVIEW: Reorder data type for better alignment
  uint8_t req_sha[ETH_ALEN];	/* sender hardware (MAC) address */
  uint8_t req_spa[4];		/* sender protocol (IP) address */
  int host_up; /* count host reply to ARP request */
  int if_index; /* interface index */
  char* file_name; /* file name to write found host */
  int ip_index; /* start index to send ARP request. End at .254 */
  double timeout_sec; /* timeout (second) for receiving reply packets. This is set for socket options */
  char if_name[IFNAMSIZ]; /* interface network to inspect */
};

extern struct proto *pr;
extern struct store *store;
