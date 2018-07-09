#include "netutils.h"

/**
 * @brief イーサネットパケットの読み込み
 * @param (oacket) パケットの構造体
 * @param (buf) ソケットから受け取ったバッファ
 * @param (size) バッファの読み込むサイズ
 */
int ReadEthernet(Packet *packet, char *buf, int size){
    // 残りポインタ
    unsigned char *ptr = buf;

    // 残りのサイズ
    int lest = size;

    if(lest < sizeof(struct ether_header)) {
        // デバッグしてデバイスナンバートサイズを表示
        return -1;
    }

    // パケットのポインタをイーサネットに
    // 無理やりキャストして代入
    packet -> eh = (struct ether_header *)ptr;

    // バッファのポインタをコピー
    packet -> buf = buf;

    // ポインタを読んだ分まで加算
    ptr += sizeof(struct ether_header);

    // 残りサイズを読んだ分減算
    lest -= sizeof(struct ether_header);

    return 0;
}
