#include "../../lib/base.h"
#include "../../lib/netutils.h"
#include "../../lib/checksum.h"
#include "../../lib/list.h"
#include "../../lib/generate.h"
#include "../../lib/print.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


/**
 * @brief 実際のパケットの受取部
 * @param (soc) ソケット
 * @return 成功したら1, 失敗したら-1
 */
int Send(int soc){
    Segment segment;
    InitSegment(&segment);

    Packet packet;
    unsigned char data[] = "hoge";
    packet.data = data;
    packet.data_size = sizeof(data);
    AddSegment(&segment, &packet);


    Packet packet2;
    unsigned char data2[] = "fuga";
    packet2.data = data2;
    packet2.data_size = sizeof(data2);
    AddSegment(&segment, &packet2);


    Packet packet3;
    unsigned char data3[] = "piyo";
    packet3.data = data3;
    packet3.data_size = sizeof(data3);
    AddSegment(&segment, &packet3);

    PrintDataStream(&segment);

    FreeSegment(&segment);

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
