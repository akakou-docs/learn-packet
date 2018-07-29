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

    // バッファのポインタをコピー
    lest_packet -> buf = ptr;

    // 残りサイズをコピー
    lest_packet -> lest = lest;
    
    // IPヘッダをコピー
    packet -> ip = iphdr;

    return 0;
}

/**
 * @brief UDPパケットの解析
 * @param (packet) パケットの構造体
 * @param (lest_packet) パケットのバイナリデータを持つ構造体
 */
int AnalyzeUDPPacket(Packet *packet, RawPacket *lest_packet){
    // 読み込んだ部分からのポインタ
    unsigned char *ptr = lest_packet -> buf;

    // 読み込む残りのサイズ
    int lest = lest_packet -> lest;

    // udpのヘッダの構造体を宣言
    struct udphdr *udphdr;

    // 構造体のサイズ分データがあるかのチェック
    if(lest < sizeof(struct udphdr)){
        // sizeが不足した場合にエラーを吐く
        fprintf(stderr, "lest(%d)<sizeof(struct udphdr)\n", lest);
        return -1;
    }

    // ptrをudphdrに無理やりキャストして代入
    udphdr = (struct udphdr *)ptr;

    // チェックサムをチェック
    // int len = ntohs(packet -> ip -> tot_len) - packet -> ip -> ihl * 4;
    // if(udphdr -> check != 0 && checkIPDATAchecksum(packet -> ip, ptr, len) == 0){
    //     packet -> udp = NULL;
    //     fprintf(stderr, "bad udp checksum\n");
    //     return -1;
    // }
    
    // ptrにパケットのデータ部を代入
    ptr += sizeof(struct udphdr);
    // lestにパケットのデータ部のサイズを代入
    lest -= sizeof(struct udphdr);

    // バッファのポインタをコピー
    lest_packet -> buf = ptr;

    // 残りサイズをコピー
    lest_packet -> lest = lest;

    // UDPヘッダをコピー
    packet -> udp = udphdr;

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
        if (AnalyzeIPPacket(packet, &lest_packet) == -1){
            return -1;
        }

        if (packet -> ip -> protocol == IPPROTO_UDP) {
            if(AnalyzeUDPPacket(packet, &lest_packet) == -1) {
                return -1;
            }
        }
    }

    packet -> data = lest_packet.buf;
    packet -> data_size = lest_packet.lest;

    return 0;
}

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
    }

    // Dataのコピー
    memcpy(buf, packet -> data, packet -> data_size);
    packet -> data = buf;
}

