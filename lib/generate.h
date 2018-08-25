#include "base.h"

#include <linux/if.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netpacket/packet.h>


void GenerateEthernetPacket(Packet *packet, struct ether_header *eh);
void AddIPHeader(Packet *packet, struct iphdr *ip);
void AddUDPHeader(Packet *packet, struct udphdr *udp);
void AddTCPHeader(Packet *packet, struct tcphdr *tcp);
void AddData(Packet *packet, unsigned char *data, size_t size);
void FreePacket(Packet *packet);
