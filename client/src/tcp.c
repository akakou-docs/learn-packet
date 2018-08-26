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
    struct ether_header eh;
    
    int size;
    Packet packet;

    // シードをセット
    srand(time(NULL));

    // パケットの初期化
    // Data
    unsigned char data[] = "hi";
    packet.data = data;
    packet.data_size = sizeof(data);

    // Ethernet
    sprintf(eh.ether_dhost, "\x00\x00\x00\x00\x00\x00");
    sprintf(eh.ether_shost, "\x00\x00\x00\x00\x00\x00");
    eh.ether_type = (u_int16_t)8;
    
    // IP
    int over_eh_size =  
            + sizeof(struct iphdr) 
            + sizeof(struct tcphdr) 
            + sizeof(data);

    struct iphdr ip = {
        sizeof(struct iphdr) / 4,
        4,
        (u_int8_t)0,
        (u_int16_t)htons(over_eh_size),
        (u_int16_t)htons(rand() % 0x10000),
        (u_int16_t)htons(0b0100000000000000),// + (rand() % 0x100)),
        (u_int8_t)1,
        (u_int8_t)IPPROTO_TCP,
        (u_int16_t)0x00,
        (u_int32_t)0x00,
        (u_int32_t)0x00        
    };

    inet_pton(AF_INET, "127.0.0.1", &(ip.saddr));
    inet_pton(AF_INET, "127.0.0.1", &(ip.daddr));

    ip.check = checksum2((unsigned char *)&ip, sizeof(struct iphdr), NULL, 0);

    // tcp
    int offset = sizeof(struct tcphdr) / 4;

    struct tcphdr tcp = {
        (u_int16_t)htons(rand() % 0x50000),
        (u_int16_t)htons(8080),
        (u_int32_t)htons(0),
        (u_int32_t)htons(0),
        (u_int8_t)0,
        (u_int8_t)offset,
        (u_int8_t)TH_SYN,
        (u_int16_t)htons(500),
        (u_int16_t)0,
        (u_int16_t)0,
     };

    // 実体の生成
    GenerateEthernetPacket(&packet, &eh);
    AddIPHeader(&packet, &ip);
    AddTCPHeader(&packet, &tcp);
    AddData(&packet, data, sizeof(data));

     // チェックサム
    int len = ntohs(packet.ip -> tot_len) - packet.ip -> ihl * 4;
    tcp.check = checksum3(packet.ip, (unsigned char *)packet.tcp, len);

    // 表示
    printf("text:\n");
    PrintEthernet(&packet);
    PrintIP(&packet);
    PrintTCP(&packet);
    PrintData(&packet);

    printf("binary:\n");
    PrintRawEthernet(&packet);
    PrintRawIP(&packet);
    PrintRawTCP(&packet);
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
        sleep(10);
    }

    // ソケットを閉じる
    close(soc);

    // 終了
    return 0;
}
