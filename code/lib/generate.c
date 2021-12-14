#include "generate.h"

#include "base.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <linux/if.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netpacket/packet.h>


void InitBaseEthernetPacket(Packet *packet, struct ether_header *eh) {
    size_t size = sizeof(struct ether_header);
    packet -> ptr = (unsigned char *)malloc(size);
    packet -> eh = (struct ether_header *)packet -> ptr;
    memcpy(packet -> eh, eh, sizeof(struct ether_header));
    packet -> size = sizeof(struct ether_header);
}
void CarryData(Packet *packet, unsigned char *data, size_t size) {
    packet -> ptr = (unsigned char *)realloc(packet -> ptr, packet -> size + size);
    packet -> data = (unsigned char *)(packet -> ptr + packet -> size);
    packet -> data_size = size;
    memcpy(packet -> data, data, size);
    packet -> size = size + packet -> size;
}

void FreePacket(Packet *packet) {
    free(packet -> ptr);
}
