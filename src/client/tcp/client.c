#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "../../print.h"


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
    packet.eh = &eh;

    // シードをセット
    srand(time(NULL));

    // パケットの初期化
    // Data
    unsigned char data[] = "hi";
    packet.data = data;
    packet.data_size = sizeof(data);

    // パケット全体のサイズ
    packet.size = sizeof(struct ether_header) 
            + sizeof(struct iphdr) 
            + sizeof(struct tcphdr) 
            + packet.data_size;

    // Ethernet
    sprintf(packet.eh -> ether_dhost, "\x64\x80\x99\x4f\x20\xf4");
    sprintf(packet.eh -> ether_shost, "\x64\x80\x99\x4f\x20\xf4");
    packet.eh -> ether_type = (u_int16_t)8;
    
    // IP
    struct iphdr ip = {
        sizeof(struct iphdr) / 4,
        4,
        (u_int8_t)0,
        (u_int16_t)htons(packet.size - sizeof(struct ether_header)),
        (u_int16_t)htons(rand() % 0x10000),
        (u_int16_t)htons(0b0100000000000000),// + (rand() % 0x100)),
        (u_int8_t)1,
        (u_int8_t)IPPROTO_TCP,
        (u_int16_t)0x00,
        (u_int32_t)0x00,
        (u_int32_t)0x00        
    };

    inet_pton(AF_INET, "192.168.10.112", &(ip.saddr));
    inet_pton(AF_INET, "192.168.10.112", &(ip.daddr));

    ip.check = checksum2((unsigned char *)&ip, sizeof(struct iphdr), NULL, 0);

    packet.ip = &ip;


    // tcp
    int offset = sizeof(struct tcphdr) / 4;
    struct tcphdr tcp = {
        (u_int16_t)htons(rand() % 0x50000),
        (u_int16_t)htons(8080),
        (u_int32_t)htons(0),
        (u_int32_t)0,
        (u_int8_t)0,
        (u_int8_t)htons(offset),
        (u_int8_t)TH_SYN,
        (u_int16_t)htons(500),
        (u_int16_t)0,
        (u_int16_t)0,
     };

    packet.tcp = &tcp;
    packet.udp = NULL;

    GeneratePacketBuffer(&packet);


    // 表示
    printf("text:\n");
    PrintEthernet(&packet);
    if (htons(packet.eh -> ether_type) == ETHERTYPE_IP) {
        PrintIP(&packet);

        if (packet.ip -> protocol == IPPROTO_UDP && packet.udp != NULL) {
            PrintUDP(&packet);
        }
        else if (packet.ip -> protocol == IPPROTO_TCP && packet.tcp != NULL) {
            PrintTCP(&packet);
        }
    }
    PrintData(&packet);

    printf("binary\n");
    PrintRawEthernet(&packet);
    if (htons(packet.eh -> ether_type) == ETHERTYPE_IP) {
        PrintRawIP(&packet);

        if (packet.ip -> protocol == IPPROTO_UDP && packet.udp != NULL) {
            PrintRawUDP(&packet);
        }
        else if (packet.ip -> protocol == IPPROTO_TCP && packet.tcp != NULL){
            PrintRawTCP(&packet);
        }
        PrintRawData(&packet);
    }
    PrintRawPacket(&packet);

    // パケットの読み込み
    if((size = write(soc, packet.ptr, packet.size)) <= 0){
        // 失敗したらエラーを開く
        perror("write");
        return -1;
    }

    free(packet.ptr);

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
        sleep(20);
    }

    // ソケットを閉じる
    close(soc);

    // 終了
    return 0;
}
