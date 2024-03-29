#include	"nmap.h"

void
send_v4(void)
{
	int			len;
    struct ether_header * eth;
    struct ether_arp *arp;

    struct sockaddr_ll device;

    eth = (struct ether_header *) sendbuf;
	arp = (struct ether_arp *) (sendbuf + sizeof(struct ether_header));

    // Source MAC Address : ARP Packet
    memcpy(arp->arp_sha, store->req_sha, ETH_ALEN);

    // Source IP Address : ARP Packet
    memcpy(arp->arp_spa, store->req_spa, 4);

    // Target Mac : Destination Mac Address : ARP Packet

    // Read more:
    // https://www.practicalnetworking.net/series/arp/arp-probe-arp-announcement/
    memset(arp->arp_tha, 0x00, ETH_ALEN);

    // Target IP : Destination IP Address : ARP Packet
    // Example: inet_pton(AF_INET, "192.168.202.129", arp->arp_tpa);

    char dst_ip[INET_ADDRSTRLEN];
    sprintf(dst_ip, "%i.%i.%i.%i", store->req_spa[0], store->req_spa[1], store->req_spa[2], store->ip_index);
    if (verbose) {
        printf("Ask: %s\n", dst_ip);
    }
    inet_pton(AF_INET, dst_ip, arp->arp_tpa);

    // Ethernet Packet
    // The ARP packet is encapsulated into an Ethernet frame with destination
    // MAC address set to ff:ff:ff:ff:ff:ff (broadcast)
    memset(eth->ether_dhost, 0xff, ETH_ALEN);   //destination address : broadcast address
    memcpy(eth->ether_shost, arp->arp_sha, ETH_ALEN); //source address
    eth->ether_type = htons(ETH_P_ARP);

    // ARP Packet
    arp->arp_hrd = htons(ARPHRD_ETHER); // 1
    arp->arp_pro = htons(ETHERTYPE_IP); // 0x0800
    arp->arp_hln = ETH_ALEN; // 6
    arp->arp_pln = 4;
    arp->arp_op = htons(ARPOP_REQUEST); // 1


    // Ethernet frame length = ethernet header (MAC + MAC + ethernet type) + ethernet data (ARP header)
    len = 6 + 6 + 2 + 28;

    device.sll_ifindex = store->if_index;
    device.sll_family = AF_PACKET;
    memcpy(device.sll_addr, store->req_sha, ETH_ALEN);
    device.sll_halen = htons(ETH_ALEN);

    store->ip_index++;

	Sendto(sockfd, sendbuf, len, 0, (struct sockaddr *) &device, sizeof (device));
}
