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



/**
 * @brief bufの生成
 * @param (packet) パケットの構造体
 */
void GeneratePacketBuffer(Packet *packet){
    // イーサネットヘッダのコピー 
    unsigned char *buf = (unsigned char *)malloc(packet -> size);
    packet -> ptr = buf;

    memcpy(buf, packet -> eh, sizeof(struct ether_header));
    packet -> eh = (struct ether_header *)buf;
    buf += sizeof(struct ether_header);

    // IPヘッダのコピー
    if (packet -> ip != NULL) {
        memcpy(buf, packet -> ip, sizeof(struct iphdr));
        packet -> ip = (struct iphdr *)buf;
    
        buf += sizeof(struct iphdr);

        // UDPヘッダのコピー
        if (packet -> udp != NULL) {
            memcpy(buf, packet -> udp, sizeof(struct udphdr));
            packet -> udp = (struct udphdr *)buf;
            buf += sizeof(struct udphdr);
        }
        // TCPヘッダのコピー
        else if (packet -> tcp != NULL) {
            memcpy(buf, packet -> tcp, sizeof(struct tcphdr));
            packet -> tcp = (struct tcphdr *)buf;
            buf += sizeof(struct tcphdr);
        }
    }

    // Dataのコピー
    if (packet -> data != NULL) {
        memcpy(buf, packet -> data, packet -> data_size);
        packet -> data = buf;
    }
}

