#include	"nmap.h"

void
proc_v4(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv)
{
    struct ether_arp *arp;

    // NOTE: Uncomment this if you want to inspect ethernet header
    // struct ether_header * eth;
	// eth = (struct ether_header *) ptr;

    // printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
    //       eth->ether_dhost[0],
    //       eth->ether_dhost[1],
    //       eth->ether_dhost[2],
    //       eth->ether_dhost[3],
    //       eth->ether_dhost[4],
    //       eth->ether_dhost[5]);

    // Skip ethernet header: 6 (ether_dhost) + 6 (ether_shost) + 2 (ether_type)
    // = 14 bytes
    arp = (struct ether_arp *) (ptr + 14);

    // printf("hrd %04X\n", htons(arp->arp_hrd));
    // printf("pro %04X\n", htons(arp->arp_pro));
    // printf("hln %02X\n", htons(arp->arp_hln));
    // printf("pln %02X\n", htons(arp->arp_pln));
    // printf("op %04X\n", htons(arp->arp_op));

    if (htons(arp->arp_op) != ARPOP_REPLY)
        return; /* not an arp reply */

    if (store->req_spa[0] != arp->arp_spa[0] && store->req_spa[1] == arp->arp_spa[1] && store->req_spa[2] == arp->arp_spa[2]) {
        return; /* not ip network we are asking */
    }
    // printf("%03d.%03d.%03d.%03d\n", store->req_spa[0], store->req_spa[1],
    // store->req_spa[2], store->req_spa[3]);

    char req_spa[INET_ADDRSTRLEN];
    sprintf(req_spa, "%i.%i.%i.%i", store->req_spa[0], store->req_spa[1], store->req_spa[2], store->req_spa[3]);
    char res_tpa[INET_ADDRSTRLEN];
    sprintf(res_tpa, "%i.%i.%i.%i", arp->arp_tpa[0], arp->arp_tpa[1], arp->arp_tpa[2], arp->arp_tpa[3]);
    char res_spa[INET_ADDRSTRLEN];
    sprintf(res_spa, "%i.%i.%i.%i", arp->arp_spa[0], arp->arp_spa[1], arp->arp_spa[2], arp->arp_spa[3]);

    if (strcmp(req_spa, res_tpa) == 0) {
        /* Check if that reply was send to us */
        store->host_up++;

        // Buffer overflow because we copied array char INET_ADDRSTRLEN size to
        // INET_ADDRSTRLEN + 1 (added \n) -> Error

        char res_spa_cp[INET_ADDRSTRLEN + 1]; /* 1 is sizeof "\n" */
        sprintf(res_spa_cp, "%s\n", res_spa);


        // Write to file with append mode
        (*pr->fwrite_file)(store->file_name, res_spa_cp, "a");

        printf("Host: %s is up\n", res_spa);
    }
}
