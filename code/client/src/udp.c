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
    struct ether_header eh;

    Packet packet;

    // シードをセット
    srand(time(NULL));

    // パケットの初期化
    // Data
    unsigned char data[] = "hello world";

    // Ethernet
    sprintf(eh.ether_dhost, "\x00\x00\x00\x00\x00\x00");
    sprintf(eh.ether_shost, "\x00\x00\x00\x00\x00\x00");
    eh.ether_type = (u_int16_t)8;
    
    // IP
    int over_eh_size =  
            + sizeof(struct iphdr) 
            + sizeof(struct udphdr) 
            + sizeof(data);

    struct iphdr ip = {
        sizeof(struct iphdr) / 4,
        4,
        (u_int8_t)0,
        (u_int16_t)htons(over_eh_size),
        (u_int16_t)htons(rand() % 0x10000),
        (u_int16_t)0,
        (u_int8_t)1,
        (u_int8_t)17,
        (u_int16_t)0x00,
        (u_int32_t)0x00,
        (u_int32_t)0x00        
    };

    inet_pton(AF_INET, "127.0.0.1", &(ip.saddr));
    inet_pton(AF_INET, "127.0.0.1", &(ip.daddr));

    ip.check = checksum2((unsigned char *)&ip, sizeof(struct iphdr), NULL, 0);

    // udp
    struct udphdr udp = {
        (u_int16_t)ntohs(rand() % 0x50000),
        (u_int16_t)ntohs(8080),
        (u_int16_t)ntohs(sizeof(udp) + sizeof(data)),
        (u_int16_t)0x00
    };

    // 実体の生成
    InitBaseEthernetPacket(&packet, &eh);
    CarryProtocolHeader(iphdr, packet, ip);
    CarryProtocolHeader(udphdr, packet, udp);
    CarryData(&packet, data, sizeof(data));

    // 表示
    printf("text:\n");
    PrintEthernet(&packet);
    PrintIP(&packet);
    PrintUDP(&packet);
    PrintData(&packet);

    printf("binary:\n");
    PrintRawEthernet(&packet);
    PrintRawIP(&packet);
    PrintRawUDP(&packet);
    PrintRawData(&packet);
    PrintRawPacket(&packet);


    // パケットの読み込み
    if((size = write(soc, packet.ptr, packet.size)) <= 0){
        // 失敗したらエラーを開く
        perror("write");
        return -1;
    }

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
