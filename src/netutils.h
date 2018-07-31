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

/**
 * @brief パケットをリストでつなぐために使う
 */
typedef struct ElementOfSegment {
    Packet *packet;
    struct ElementOfSegment *fd;
    struct ElementOfSegment *bk;
} ElementOfSegment;


/**
 * @brief 読み込み時のセグメント
 */
typedef struct {
    unsigned char *ptr;
    struct ElementOfSegment *start;
    struct ElementOfSegment *end;
} Segment;


int InitRawSocket(char *device);
