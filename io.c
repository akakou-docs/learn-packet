#include <stdlib.h>

#include "netutils.h"
#include "checksum.h"

/**
 * @brief イーサネットパケットの解析
 * @param (packet) パケットの構造体
 * @param (lest_packet) パケットのバイナリデータを持つ構造体
 */
int AnalyzeEtherPacket(Packet *packet, RawPacket *lest_packet){
    // 残りポインタ
    unsigned char *ptr = lest_packet -> buf;

    // 残りのサイズ
    int lest = lest_packet -> lest;

    if(lest < sizeof(struct ether_header)) {
        // デバッグしてデバイスナンバートサイズを表示
        return -1;
    }

    // パケットのポインタをイーサネットに
    // 無理やりキャストして代入
    packet -> eh = (struct ether_header *)ptr;

    // ポインタを読んだ分まで加算
    ptr += sizeof(struct ether_header);

    // 残りサイズを読んだ分減算
    lest -= sizeof(struct ether_header);

    // バッファのポインタをコピー
    lest_packet -> buf = ptr;

    // 残りサイズをコピー
    lest_packet -> lest = lest;

    return 0;
}

/**
 * @brief IPパケットの解析
 * @param (packet) パケットの構造体
 * @param (lest_packet) パケットのバイナリデータを持つ構造体
 */
int AnalyzeIPPacket(Packet *packet, RawPacket *lest_packet){
    // 読み込んだ部分からのポインタ
    unsigned char *ptr = lest_packet -> buf;
    
    // 読み込んでない残りのサイズ
    int lest = lest_packet -> lest;
    
    // ipのパケットの構造体を宣言
    struct iphdr *iphdr;

    // オプション
    unsigned char *option;

    // オプションの長さ、パケッtの長さ
    int optionLen, len;
    
    // チェックサム
    unsigned short sum;

    // 構造体のサイズ分データがあるかのチェック
    if(lest < sizeof(struct iphdr)){
        // sizeが不足した場合にエラーを吐く
        fprintf(stderr, "lest(%d)<sizeof(struct iphdr)\n", lest);
        return -1;
    }

    // ptrをiphdrに無理やりキャストして代入
    iphdr = (struct iphdr *)ptr;
    
    // ptrにパケットのデータ部を代入
    ptr += sizeof(struct iphdr);
    // lestにパケットのデータサイズを代入
    lest -= sizeof(struct iphdr);

    // オプションの大きさ(IPヘッダ長を4で割った値 * 4 - ヘッダの大きさ)を格納
    optionLen = iphdr -> ihl * 4 - sizeof(struct iphdr);
    if(optionLen > 0){
        if(optionLen >= 1500){
            // オプションのサイズが大きすぎたらエラーを吐く
            fprintf(stderr, "IP optionLen(%d):too big\n", optionLen);
            return -1;
        }
        // サイズがちょうどよかったら
        // ptrの値をoption に代入
        option = ptr;
        // ptrをoptionLen分読み進める
        ptr += optionLen;
        // lestをoptionLen分減らす
        lest -= optionLen;
    }

    // IPのチェックサムの確認
    if(checkIPchecksum(iphdr, option, optionLen) == 0){
        // チェックサムを見てパケットが壊れたら、エラーを吐く
        fprintf(stderr, "bad ip checksum\n");
        return -1;
    }

    // IPヘッダを表示する
    packet -> ip = iphdr;

    return 0;
}

/**
 * @brief パケットの解析
 * @param (packet) パケットの構造体
 * @param (lest_packet) パケットのバイナリデータを持つ構造体
 */
int AnalyzePacket(Packet *packet){
    RawPacket lest_packet = {
        packet -> ptr,
        packet -> size
    };
    
    AnalyzeEtherPacket(packet, &lest_packet);
    
    if (htons(packet -> eh -> ether_type) == ETHERTYPE_IP){
        AnalyzeIPPacket(packet, &lest_packet);
    }
}

/**
 * @brief イーサネットパケットの生成
 * @param (lest_packet) パケットのバイナリデータを持つ構造体
 * @param (packet) パケットの構造体
 */
void GeneratePacketBuffer(Packet *packet){
    size_t size = sizeof(struct ether_header);
    unsigned char *buf = (unsigned char *)malloc(size);
    
    memcpy(buf, packet -> eh, size);
    
    packet -> ptr = buf;
    packet -> size = size;
}

