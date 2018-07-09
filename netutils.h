#include "netutils.c"

/**
 * @brief パケットのデータ
 */
typedef struct {
    int size;
    unsigned char *buf;
    struct ether_header *eh;
    //struct iphdr *iphdr;
    //struct tcphdr *tcphdr;
} Packet;


int InitRawSocket(char *device);
char *my_ether_ntoa_r(unsigned char *hwaddr, char *buf, socklen_t size);
