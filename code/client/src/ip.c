#include "../../lib/base.h"
#include "../../lib/netutils.h"
#include "../../lib/checksum.h"
#include "../../lib/generate.h"
#include "../../lib/print.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>



/**
 * @brief 実際のパケットの受取部
 * @param (soc) ソケット
 * @return 成功したら1, 失敗したら-1
 */
int Send(int soc){
    // 変数の宣言
    int size;
    Packet packet;

    struct ether_header eh;

    // シードをセット
    srand(time(NULL));

    // パケットの初期化
    // Ethernet
    sprintf(eh.ether_dhost, "\x00\x00\x00\x00\x00\x00");
    sprintf(eh.ether_shost, "\x00\x00\x00\x00\x00\x00");
    eh.ether_type = (u_int16_t)8;
    
    // IP
    struct iphdr ip = {
        sizeof(struct iphdr) / 4,
        4,
        (u_int8_t)0,
        (u_int16_t)htons(sizeof(struct iphdr)),
        (u_int16_t)htons(rand() % 0x10000),
        (u_int16_t)0,
        // htons(0b01000000000000000000000 + (rand() % 0x100)),
        (u_int8_t)1,
        (u_int8_t)17,
        (u_int16_t)0x00,
        (u_int32_t)0x00,
        (u_int32_t)0x00        
    };

    inet_pton(AF_INET, "127.0.0.1", &(ip.saddr));
    inet_pton(AF_INET, "127.0.0.1", &(ip.daddr));

    ip.check = checksum2((unsigned char *)&ip, sizeof(struct iphdr), NULL, 0);

    // パケットの実態の生成
    InitBaseEthernetPacket(&packet, &eh);
    CarryProtocolHeader(iphdr, packet, ip);

    // パケットの表示
    PrintEthernet(&packet);
    PrintIP(&packet);
    PrintRawEthernet(&packet);    
    PrintRawIP(&packet);
    PrintRawPacket(&packet);

    // パケットの送信
    if((size = write(soc, packet.ptr, packet.size)) <= 0){
        // 失敗したらエラーを開く
        perror("write");
        return -1;
    }

    // パケットの開放
    FreePacket(&packet);

    return 0;
}


/**
 * @brief メイン関数
 * @param (argc) 引数の個数
 * @param (argv) 引数の配列
 * @param (envp) 環境変数
 * @return 常に0
 */
int main(int argc, char *argv[], char *envp[]){
    int soc;

    if (argc <= 1){
        // 引数が足りない場合はエラーを吐く
        fprintf(stderr, "ltest device-name\n");
    }

    // ソケットを用意する
    if ((soc = InitRawSocket(argv[1])) == -1){
        // 失敗したらエラーを吐く
        fprintf(stderr, "InitRawSocket:error:%s\n", argv[1]);
        return -1;
    }

    // 無限に実行
    while(1){
        Send(soc);
        sleep(5);
    }

    // ソケットを閉じる
    close(soc);

    // 終了
    return 0;
}
