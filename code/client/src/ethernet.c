#include "../../lib/base.h"
#include "../../lib/netutils.h"
#include "../../lib/checksum.h"
#include "../../lib/generate.h"
#include "../../lib/print.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/**
 * @brief 実際のパケットの受取部
 * @param (soc) ソケット
 * @return 成功したら1, 失敗したら-1
 */
int Send(int soc){
    // 変数の宣言
    Packet packet;
    int size;
    struct ether_header eh;

    // パケットの初期化
    sprintf(eh.ether_dhost, "\x00\x00\x00\x00\x00\x00");
    sprintf(eh.ether_shost, "\x00\x00\x00\x00\x00\x00");
    eh.ether_type = (u_int16_t)8;

    InitBaseEthernetPacket(&packet, &eh);

    PrintEthernet(&packet);
    PrintRawPacket(&packet);

    // パケットの読み込み
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
