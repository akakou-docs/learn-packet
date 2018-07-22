#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../print.h"


/**
 * @brief 実際のパケットの受取部
 * @param (soc) ソケット
 * @return 成功したら1, 失敗したら-1
 */
int Send(int soc){
    // 変数の宣言
    Packet packet;
    int size;
    unsigned char buf[100];

    // パケットの宣言
    union {
        char buf[sizeof(struct ether_header)];
        struct ether_header hdr;
    } eth_hdr;

    // パケットの初期化
    packet.eh = &eth_hdr.hdr;

    str_to_ether("64:80:99:4f:20:f4", packet.eh -> ether_dhost);
    str_to_ether("64:80:99:4f:20:f4", packet.eh -> ether_shost);

    packet.eh -> ether_type = 8;

    // パケットの読み込み
    if((size = write(soc, eth_hdr.buf, sizeof(buf))) <= 0){
        // 失敗したらエラーを開く
        perror("write");
        return -1;
    }

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
    //while(1){
        Send(soc);
    //}

    // ソケットを閉じる
    close(soc);

    // 終了
    return 0;
}
