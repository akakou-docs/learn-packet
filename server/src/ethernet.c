#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/base.h"
#include "../../lib/netutils.h"
#include "../../lib/checksum.h"
#include "../../lib/list.h"
#include "../../lib/analyze.h"
#include "../../lib/print.h"



/**
 * @brief 実際のパケットの受取部
 * @param (soc) ソケット
 * @return 成功したら1, 失敗したら-1
 */
int Serv(int soc){
    // 変数の宣言
    Packet packet;
    unsigned char buf[2048];

    // パケットの読み込み
    if((packet.size = read(soc, buf, sizeof(buf))) <= 0){
        // 失敗したらエラーを開く
        perror("read");
        return -1;
    }

    packet.ptr = buf;

    // パケットを構造体に当てはめる
    AnalyzePacket(&packet);

    // 表示
    PrintEthernet(&packet);
    PrintRawEthernet(&packet);
        
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
        Serv(soc);
    }

    // ソケットを閉じる
    close(soc);
    // 終了
    return 0;
}
