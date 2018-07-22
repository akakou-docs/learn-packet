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
    int size;
    unsigned char *buf;
} RawPacket;

int InitRawSocket(char *device);
char *ether_to_str(unsigned char *binary, char *string, socklen_t size);
uint8_t *str_to_ether(char *string, unsigned char *binary);
