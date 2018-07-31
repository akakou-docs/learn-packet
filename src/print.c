#include "io.h"
#include "list.c"

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

void PrintString(RawPacket raw_packet) {
    char *data = (char *)malloc(raw_packet.lest + sizeof(char));
    memcpy(data, raw_packet.buf, raw_packet.lest);
    data[raw_packet.lest + 1] = 0x0;

    printf("%s", data);

    free(data);
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

    printf("source port      :  %d\n", htons(packet -> tcp -> th_sport));
    printf("destination port :  %d\n", htons(packet -> tcp -> th_dport));
    printf("seq number       :  %d\n", htons(packet -> tcp -> th_seq));
    printf("ack number       :  %d\n", htons(packet -> tcp -> th_ack));

    printf("data offset      :  %d\n", htons(packet -> tcp -> th_off));
    printf("res              : x%x\n", htons(packet -> tcp -> th_x2));
    
    printf("flag             : x%x\n", htons(packet -> tcp -> th_flags));

    printf("window           :  %d\n", htons(packet -> tcp -> th_win));
    printf("check            : x%x\n", htons(packet -> tcp -> th_sum));
    printf("urg_ptr          : x%x\n", htons(packet -> tcp -> th_urp));
}

void PrintData(Packet *packet){
    printf("-*-*-*-*-   DATA   -*-*-*-*-\n");
    if (packet -> data_size <= 0) {
        return;
    }

    RawPacket raw_packet = {
        packet -> data,
        packet -> data_size
    };

    PrintString(raw_packet);
    printf("\n\n");
}

int PrintProtocolStream(Segment *segment) {
    printf("-*-*-* Protocol Stream *-*-*-\n");

    if (segment -> start == NULL || segment -> end == NULL) {
        return -1;
    }

    InitIterater(segment);
    while(1) {
        ElementOfSegment *element = Iterate();

        if (element == NULL) {
            break;
        }

        RawPacket raw_packet = {
            element -> packet -> data,
            element -> packet -> data_size
        };

        PrintString(raw_packet);
    }

    printf("\n\n");

    
    return 0;
}