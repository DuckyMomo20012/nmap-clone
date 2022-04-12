#include	"nmap.h"

void
readloop(void)
{
	int				size;
	char			recvbuf[BUFSIZE];
	char			controlbuf[BUFSIZE];
	struct msghdr	msg;
	struct iovec	iov;
	ssize_t			n;
	struct timeval	tval;
	struct timeval	timeout;

	// sockfd = Socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	sockfd = Socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	setuid(getuid());		/* don't need special permissions any more */
	if (pr->finit)
		(*pr->finit)();

	// setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));


    // Set timeout for our program
    timeout.tv_sec = store->timeout_sec;
    timeout.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    struct ifreq ifr;

    sprintf(ifr.ifr_name, "%s", store->ifname);

    // strcpy(ifr.ifr_name, store->ifname);

    //Get interface index using name
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1) {
        err_sys("SIOCGIFINDEX");
    }
    int ifindex = ifr.ifr_ifindex;



    //Get MAC address of the interface
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1) {
        err_sys("SIOCGIFHWADDR");
    }

    //Copy mac address to output
    unsigned char mac[6];
    memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);

    // Copy mac address to proto
    memcpy (store->req_sha, ifr.ifr_hwaddr.sa_data, 6);

    //Get IP address of the interface
    if (ioctl(sockfd, SIOCGIFADDR, &ifr) == -1) {
        err_sys("SIOCGIFADDR");
    }
    //Print IP address
    struct sockaddr_in* ipaddr = (struct sockaddr_in*)&ifr.ifr_addr;


    // Copy ip address to proto
    // 2 first bytes is empty
    memcpy(store->req_spa, ifr.ifr_addr.sa_data + 2, 4);

    // NOTE: When printf, remember to add "\n"!!!
    if (verbose) {
        printf("Interface index is %d\n", ifindex);

        printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
            mac[0],
            mac[1],
            mac[2],
            mac[3],
            mac[4],
            mac[5]);


        printf("IP address: %s\n", inet_ntoa(ipaddr->sin_addr));
    }


    for ( ; store->ip_index < 255 ; ) {
	    sig_alrm(SIGALRM);		/* send packet */
        // REVIEW: kinda weird, should fix this later
        // alarm(1); /* Then schedule it to run after 1 sec */
    }

	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);
	msg.msg_name = pr->sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = controlbuf;
	for ( ; ; ) {
		msg.msg_namelen = pr->salen;
		msg.msg_controllen = sizeof(controlbuf);

		n = recvmsg(sockfd, &msg, 0);
		if (n < 0) {
			if (errno == EINTR)
				continue;
            else if (errno == EAGAIN) {
                // Ring, ring, ring timeout!
                if (verbose) {
                    printf("Socket automatically timeout after %.1f secs of nonactivity\n", store->timeout_sec);
                }
                exit(0);
            }
			else
				err_sys("recvmsg error");
		}

        Gettimeofday(&tval, NULL);

        // Process reply packet
		(*pr->fproc)(recvbuf, n, &msg, &tval);
	}
}
