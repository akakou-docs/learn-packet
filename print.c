#include "io.h"


/**
 * @brief パケットのバイナリを表示
 * @param (packet) パケット
 */
void PrintRawPacket(RawPacket *raw_packet){
    int count = 0;

    // 残りポインタ
    unsigned char *buf = raw_packet -> buf;

    // 残りのサイズ
    int lest = raw_packet -> size;
    printf("size:%d\n\n", lest);

    // バイナリを表示する
    while (0 <= lest) {
        printf("%2x ", *buf);
        buf += sizeof(unsigned char);
        lest -= sizeof(unsigned char);
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


    printf("destionation : %s\n", dhost);
    printf("source       : %s\n", shost);
    printf("type         : %d\n\n", packet -> eh -> ether_type);
}
