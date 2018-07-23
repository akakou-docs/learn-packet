#include "netutils.c"

/**
 * @brief パケットのデータ
 */
typedef struct {
    struct ether_header *eh;
    //struct iphdr *iphdr;
    //struct tcphdr *tcphdr;
} Packet;

/**
 * @brief パケットのバイナリデータ
 */
typedef struct {
    unsigned char *buf;
    size_t size;
} RawPacket;

int InitRawSocket(char *device);
