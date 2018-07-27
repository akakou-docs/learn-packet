#include "io.h"


/**
 * @brief バイナリを表示
 * @param (raw_packet) パケット
 */

void PrintBinary(RawPacket raw_packet) {
    int count = 0;
    
    printf("size:%d\n\n", raw_packet.lest);

    // バイナリを表示する
    while (0 < raw_packet.lest) {
        printf("%02x ", *(raw_packet.buf));
        raw_packet.buf += sizeof(unsigned char);
        raw_packet.lest -= sizeof(unsigned char);
        count ++;

        if (count % 5 == 0) {
            // バイナリ5つにうち1回
            // 改行を入れる
            putchar('\n');
        }
    }

    puts("\n");
}

/**
 * @brief パケットのバイナリを表示
 * @param (packet) パケット
 */
void PrintRawPacket(Packet *packet){
    int count = 0;

    RawPacket raw_packet = {
        packet -> ptr,
        packet -> size
    };

    printf("-*-*-*-*-  Raw Packet  -*-*-*-*-\n");

    PrintBinary(raw_packet);
}

/**
 * @brief パケットのバイナリを表示
 * @param (packet) パケット
 */
void PrintRawEthernet(Packet *packet){
    int count = 0;

    RawPacket raw_packet = {
        (unsigned char *)packet -> eh,
        sizeof(struct ether_header)
    };

    printf("-*-*-*-*- Raw Ethernet -*-*-*-*-\n");

    PrintBinary(raw_packet);
}

/**
 * @brief パケットのバイナリを表示
 * @param (packet) パケット
 */
void PrintRawIP(Packet *packet){
    int count = 0;

    RawPacket raw_packet = {
        (unsigned char *)packet -> ip,
        sizeof(struct iphdr)
    };

    printf("-*-*-*-*-    Raw IP    -*-*-*-*-\n");

    PrintBinary(raw_packet);
}

/**
 * @brief イーサネットを表示する
 * @param (packet) パケット
 */
void PrintEthernet(Packet *packet){
    int count = 0;

    char dhost[19];
    char shost[19];

    snprintf(dhost, sizeof(dhost), "%02x:%02x:%02x:%02x:%02x:%02x",
        packet -> eh -> ether_dhost[0],
        packet -> eh -> ether_dhost[1],
        packet -> eh -> ether_dhost[2],
        packet -> eh -> ether_dhost[3],
        packet -> eh -> ether_dhost[4],
        packet -> eh -> ether_dhost[5]
    );

    snprintf(shost, sizeof(shost), "%02x:%02x:%02x:%02x:%02x:%02x",
        packet -> eh -> ether_shost[0],
        packet -> eh -> ether_shost[1],
        packet -> eh -> ether_shost[2],
        packet -> eh -> ether_shost[3],
        packet -> eh -> ether_shost[4],
        packet -> eh -> ether_shost[5]
    );

    printf("-*-*-*-*- Ethernet -*-*-*-*-\n");
    printf("destionation : %s\n", dhost);
    printf("source       : %s\n", shost);
    printf("type         : %d\n\n", packet -> eh -> ether_type);
}

void PrintIP(Packet *packet){
    char saddr[12];
    char daddr[12];

    sprintf(saddr, "%d.%d.%d.%d", 
        (packet -> ip -> saddr)&0xFF,
        (packet -> ip -> saddr >> 8)&0xFF,
        (packet -> ip -> saddr >> 16)&0xFF,
        (packet -> ip -> saddr >> 24)&0xFF
    );

    sprintf(daddr, "%d.%d.%d.%d", 
        (packet -> ip -> daddr)&0xFF,
        (packet -> ip -> daddr >> 8)&0xFF,
        (packet -> ip -> daddr >> 16)&0xFF,
        (packet -> ip -> daddr >> 24)&0xFF
    );

    printf("-*-*-*-*-    IP    -*-*-*-*-\n");
    printf("version          : %d\n", packet -> ip -> version);
    printf("IP header length : %d\n", packet -> ip -> ihl);
    printf("type of service  : %d\n", packet -> ip -> tos);
    printf("total length     : %d\n", packet -> ip -> tot_len);
    printf("identification   : %d\n", packet -> ip -> id);
    printf("frag offset      : %d\n", packet -> ip -> frag_off);
    printf("time to live     : %d\n", packet -> ip -> ttl);
    printf("protocol         : %d\n", packet -> ip -> protocol);
    printf("checksum         : %d\n", packet -> ip -> check);
    printf("destionation     : %s\n", daddr);
    printf("source           : %s\n\n", saddr);
}