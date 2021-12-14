#include "base.h"

#include <linux/if.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netpacket/packet.h>

#define CarryProtocolHeader(headerType, packet, header) \
    { \
        size_t size = sizeof(struct headerType); \
        packet.ptr = (unsigned char *)realloc(packet.ptr, packet.size + size);\
        packet.header = (struct headerType *)(packet.ptr + packet.size); \
        memcpy(packet.header, &header, sizeof(struct headerType)); \
        packet.size = size + packet.size; \
    } \

void InitBaseEthernetPacket(Packet *packet, struct ether_header *eh);
void CarryData(Packet *packet, unsigned char *data, size_t size);
void FreePacket(Packet *packet);
