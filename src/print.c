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
    RawPacket raw_packet = {
        (unsigned char *)packet -> eh,
        sizeof(struct ether_header)
    };

    printf("-*-*-*-*- Raw Ethernet -*-*-*-*-\n");

    PrintBinary(raw_packet);
}

/**
 * @brief IPのバイナリを表示
 * @param (packet) パケット
 */
void PrintRawIP(Packet *packet){
    RawPacket raw_packet = {
        (unsigned char *)packet -> ip,
        sizeof(struct iphdr)
    };

    printf("-*-*-*-*-    Raw IP    -*-*-*-*-\n");

    PrintBinary(raw_packet);
}

/**
 * @brief UDPのバイナリを表示
 * @param (packet) パケット
 */
void PrintRawUDP(Packet *packet){
    RawPacket raw_packet = {
        (unsigned char *)packet -> udp,
        sizeof(struct udphdr)
    };

    printf("-*-*-*-*-   Raw UDP   -*-*-*-*-\n");

    PrintBinary(raw_packet);
}

/**
 * @brief UDPのバイナリを表示
 * @param (packet) パケット
 */
void PrintRawTCP(Packet *packet){
    RawPacket raw_packet = {
        (unsigned char *)packet -> tcp,
        sizeof(struct tcphdr)
    };

    printf("-*-*-*-*-   Raw TCP   -*-*-*-*-\n");

    PrintBinary(raw_packet);
}

/**
 * @brief データ部のバイナリを表示
 * @param (packet) パケット
 */
void PrintRawData(Packet *packet){
    RawPacket raw_packet = {
        (unsigned char *)packet -> data,
        packet -> data_size
    };

    printf("-*-*-*-*-   Raw DATA   -*-*-*-*-\n");

    PrintBinary(raw_packet);
}


/**
 * @brief イーサネットを表示する
 * @param (packet) パケット
 */
void PrintEthernet(Packet *packet){
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
    char saddr[15];
    char daddr[15];

    inet_ntop(AF_INET, &(packet -> ip -> saddr), saddr, sizeof(saddr));
    inet_ntop(AF_INET, &(packet -> ip -> daddr), daddr, sizeof(daddr));

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

void PrintUDP(Packet *packet) {
    printf("-*-*-*-*-   UDP   -*-*-*-*-\n");

    printf("source port      : %d\n", htons(packet -> udp -> uh_sport));
    printf("destination port : %d\n", htons(packet -> udp -> uh_dport));
    printf("udp length       : %d\n", htons(packet -> udp -> uh_ulen));
    printf("checksum         : %x\n\n", htons(packet -> udp -> uh_sum));
}

void PrintTCP(Packet *packet) {
    printf("-*-*-*-*-   TCP   -*-*-*-*-\n");

    printf("source   : %d\n", htons(packet -> tcp -> source));
    printf("dest     : %d\n", htons(packet -> tcp -> dest));
    printf("seq      : %d\n", htons(packet -> tcp -> seq));
    printf("ack_seq  : %x\n", htons(packet -> tcp -> ack_seq));

    printf("doff     : %x\n", htons(packet -> tcp -> doff));
    printf("res1     : %x\n", htons(packet -> tcp -> res1));
    printf("res2     : %x\n", htons(packet -> tcp -> res2));
    printf("urg      : %x\n", htons(packet -> tcp -> urg));
    printf("ack      : %d\n", htons(packet -> tcp -> ack));
    printf("psh      : %d\n", htons(packet -> tcp -> psh));
    printf("rst      : %d\n", htons(packet -> tcp -> rst));
    printf("syn      : %d\n", htons(packet -> tcp -> syn));
    printf("fin      : %d\n", htons(packet -> tcp -> fin));

    printf("window   : %x\n", htons(packet -> tcp -> window));
    printf("check    : %x\n", htons(packet -> tcp -> check));
    printf("urg_ptr  : %x\n", htons(packet -> tcp -> urg_ptr));
}

void PrintData(Packet *packet){
    printf("-*-*-*-*-   DATA   -*-*-*-*-\n");
    if (packet -> data_size <= 0) {
        return;
    }

    char *data = (char *)malloc(packet -> size + 1);
    memcpy(data, packet -> data, packet -> data_size);
    data[packet -> data_size + 1] = 0x0;

    printf("%s\n\n", data);

    free(data);
}
