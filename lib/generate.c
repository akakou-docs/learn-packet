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


void GenerateEthernetPacket(Packet *packet, struct ether_header *eh) {
    size_t size = sizeof(struct ether_header);
    packet -> ptr = (unsigned char *)malloc(size);
    packet -> eh = (struct ether_header *)packet -> ptr;
    memcpy(packet -> eh, eh, sizeof(struct ether_header));
    packet -> size = sizeof(struct ether_header);
}

void AddIPHeader(Packet *packet, struct iphdr *ip) {
    size_t size = sizeof(struct iphdr);
    packet -> ptr = (unsigned char *)realloc(packet -> ptr, packet -> size + size);
    packet -> ip = (struct iphdr *)(packet -> ptr + packet -> size);
    memcpy(packet -> ip, ip, sizeof(struct iphdr));
    packet -> size = size + packet -> size;
}

void AddUDPHeader(Packet *packet, struct udphdr *udp) {
    size_t size = sizeof(struct udphdr);
    packet -> ptr = (unsigned char *)realloc(packet -> ptr, packet -> size + size);
    packet -> udp = (struct udphdr *)(packet -> ptr + packet -> size);
    memcpy(packet -> udp, udp, sizeof(struct udphdr));
    packet -> size = size + packet -> size;
}

void AddTCPHeader(Packet *packet, struct tcphdr *tcp) {
    size_t size = sizeof(struct tcphdr);
    packet -> ptr = (unsigned char *)realloc(packet -> ptr, packet -> size + size);
    packet -> tcp = (struct tcphdr *)(packet -> ptr + packet -> size);
    memcpy(packet -> tcp, tcp, sizeof(struct tcphdr));
    packet -> size = size + packet -> size;
}

void AddData(Packet *packet, unsigned char *data, size_t size) {
    packet -> ptr = (unsigned char *)realloc(packet -> ptr, packet -> size + size);
    packet -> data = (unsigned char *)(packet -> ptr + packet -> size);
    packet -> data_size = size;
    memcpy(packet -> data, data, size);
    packet -> size = size + packet -> size;
}

void FreePacket(Packet *packet) {
    free(packet -> ptr);
    memset(packet, 0, sizeof(Packet));
}
