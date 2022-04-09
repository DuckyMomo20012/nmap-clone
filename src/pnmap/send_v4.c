#include	"ping.h"

void
send_v4(void)
{
	int			len;
    struct ether_header * eth;
    struct ether_arp *arp;

    struct sockaddr_ll device;

    eth = (struct ether_header *) sendbuf;
	arp = (struct ether_arp *) (sendbuf + sizeof(struct ether_header));

    //Source MAC Address : ARP Packet
    memcpy(arp->arp_sha, pr->pr_sha, ETH_ALEN);

    //Source IP Address : ARP Packet
    memcpy(arp->arp_spa, pr->pr_spa, 4);

    //Target Mac : Destination Mac Address : ARP Packet
    memset(arp->arp_tha, 0x00, ETH_ALEN);

    //Target IP : Destination IP Address : ARP Packet
    // Example:
    // inet_pton(AF_INET, "192.168.202.129", arp->arp_tpa);

    char dst_ip[INET_ADDRSTRLEN];
    sprintf(dst_ip, "%i.%i.%i.%i", pr->pr_spa[0], pr->pr_spa[1], pr->pr_spa[2], pr->ip_index);
    printf("Ask: %s\n", dst_ip);
    inet_pton(AF_INET, dst_ip, arp->arp_tpa);

    //Ethernet Packet
    memset(eth->ether_dhost, 0xff, ETH_ALEN);   //destination address : broadcast address
    memcpy(eth->ether_shost, arp->arp_sha, ETH_ALEN); //source address
    eth->ether_type = htons(ETH_P_ARP);

    // ARP Packet
    // arp->ea_hdr.ar_hrd = ARPHRD_ETHER;
    arp->arp_hrd = htons(ARPHRD_ETHER); // 1
    arp->arp_pro = htons(ETHERTYPE_IP); // 0x0800
    arp->arp_hln = ETH_ALEN; // 6
    arp->arp_pln = 4;
    arp->arp_op = htons(ARPOP_REQUEST); // 1


    // Ethernet frame length = ethernet header (MAC + MAC + ethernet type) + ethernet data (ARP header)
    len = 6 + 6 + 2 + 28;

    device.sll_ifindex = 2;
    device.sll_family = AF_PACKET;
    memcpy (device.sll_addr, pr->pr_sha, 6 * sizeof (uint8_t));
    device.sll_halen = htons (6);

    pr->ip_index++;

	Sendto(sockfd, sendbuf, len, 0, (struct sockaddr *) &device, sizeof (device));
}
