#include "io.h"


/**
 * @brief パケットのバイナリを表示
 * @param (packet) パケット
 */
void PrintPacket(Packet *packet){
    int count = 0;

    // 残りポインタ
    unsigned char *buf = packet -> buf;

    // 残りのサイズ
    int lest = packet -> size;
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

    char mac[19];

    printf("destionation : %s\n", my_ether_ntoa_r(packet -> eh -> ether_dhost, mac, sizeof(mac)));
    printf("source       : %s\n", my_ether_ntoa_r(packet -> eh -> ether_shost, mac, sizeof(mac)));
    printf("type         : %d\n\n", packet -> eh -> ether_type);
}
