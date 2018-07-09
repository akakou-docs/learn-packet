#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "print.h"


/**
 * @brief 実際のパケットの受取部
 * @param (soc) ソケット
 * @return 成功したら1, 失敗したら-1
 */
int serv(int soc){
    // 変数の宣言
    Packet packet;
    int size;
    unsigned char buf[2048];

    // パケットの読み込み
    if((size = read(soc, buf, sizeof(buf))) <= 0){
        // 失敗したらエラーを開く
        perror("read");
        return -1;
    }

    packet.size = sizeof(buf);

    // パケットを構造体に当てはめる
    ReadEthernet(&packet, buf, packet.size);

    // 表示
    PrintEthernet(&packet);
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
        serv(soc);
    }

    // ソケットを閉じる
    close(soc);
    // 終了
    return 0;
}
