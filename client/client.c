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
    int size;
    unsigned char buf[100];

    struct ether_header eh;

	RawPacket raw_packet = {
        buf,
        sizeof(buf)
    };

    Packet packet = {
        &(eh)
    };

    // パケットの初期化
    sprintf(packet.eh -> ether_dhost, "\x64\x80\x99\x4f\x20\xf4");
    sprintf(packet.eh -> ether_shost, "\x64\x80\x99\x4f\x20\xf4");
    packet.eh -> ether_type = (u_int16_t)8;

    PrintEthernet(&packet);

    GenerateRawEtherPacket(&raw_packet, &packet);

    // パケットの読み込み
    if((size = write(soc, raw_packet.buf, raw_packet.size)) <= 0){
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
    while(1){
        Send(soc);
        sleep(5);
    }

    // ソケットを閉じる
    close(soc);

    // 終了
    return 0;
}
