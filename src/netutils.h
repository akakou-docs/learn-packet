#include "netutils.c"


/**
 * @brief 読み込み時のパケット
 */
typedef struct {
    unsigned char *buf;
    int lest;
} RawPacket;

/**
 * @brief パケットのデータ
 */
typedef struct {
    unsigned char *ptr;
    int size;
    struct ether_header *eh;
    struct iphdr *ip;
    struct tcphdr *tcp;
    struct udphdr *udp;
    unsigned char *data;
    int data_size;
} Packet;


int InitRawSocket(char *device);
