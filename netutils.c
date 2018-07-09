#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <netinet/if_ether.h>


/**
 * @brief RAWソケットを準備する
 * @param (device) ネットワークインターフェース名
 * @return socketのディスクリプタ
 */
int InitRawSocket(char *device) {
    // ネットワークデバイスを設定する際に
    // ioctl というシステムコール時に渡して
    // 設定や情報取得を行うための構造体
    struct ifreq ifreq;
    // デバイスに依存しない物理層のアドレスを格納するプロトコル
    struct sockaddr_ll sa;
    // ソケット(データリンク層を扱うディスクリプタ)
    int soc;

    // ソケットを用意
    // 第一引数 : プロトコルファミリ→ PF_PACKET(L2)
    // 第二引数 : 通信方式→ SOCK_RAW
    // 第三引数 : データリンク層→ ETH_P_IP (IPパケットのみ)
    if((soc = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP))) < 0) {
        fprintf(stderr, "[init] socket error\n");
        return -1;
    }

    // ifreqをゼロ初期化
    memset(&ifreq, 0, sizeof(struct ifreq));
    // ネットワークデバイス名をifreq.ifr_nameにコピー
    strncpy(ifreq.ifr_name, device, sizeof(ifreq.ifr_name) -1);
    // インターフェース名に対応した
    // インターフェースのインデックスを取得
    // 第一引数 : オープンされたディスクリプタ
    // 第二引数 : デバイスドライバに対する個々の操作に割り当てられた番→ SIOCGIFINDEX
    // インデックスの流し先→ ifreq
    if(ioctl(soc, SIOCGIFINDEX, &ifreq) < 0) {
        fprintf(stderr, "[init] ioctl error\n");
        close(soc);
        return -1;
    }

    // sa.sll_family (デバイスに依存しない物理層の構造体のプロトコルファミリ部)に、
    // PF_PACKET (L2のプロトコルファミリ)を設定
    sa.sll_family = PF_PACKET;
    // sa.sll_protocol (デバイスに依存しない物理層の構造体のプロトコル部)に、
    // ホストバイトオーダからネットワークバイトオーダに変換した、
    // IPパケットのみを取得することを示したデータを格納
    sa.sll_protocol = htons(ETH_P_IP);
    // ifreq.ifr_ifindex に格納されたインターフェース番号をsa.sll_ifindexに設定
    sa.sll_ifindex = ifreq.ifr_ifindex;

    // 第一引数で指定されたディスクリプターで参照されるソケットに、
    // 第二引数で指定されたアドレスを割り当てる。
    // 第一引数 : 元のディスクリプタ
    // 第二引数 : 割り当て先
    // 第三引数 : 第二引数の指す構造体のサイズ
    if(bind(soc, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        fprintf(stderr, "[init] bind error\n");
        close(soc);
        return -1;
    }

    return soc;
}

/**
 * @brief MACアドレスを文字列に変換
 * @param (hwaddr) MACアドレスのポインタ
 * @param (buf) 出力先
 * @param (size) 書き込むサイズ
 * @return (buf) 文字列として出力されるMACアドレス
 */
char *my_ether_ntoa_r(uint8_t *hwaddr, char *buf, socklen_t size){
        snprintf(buf, size, "%02x:%02x:%02x:%02x:%02x:%02x",
        hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);

    return(buf);
}
