#include <linux/if.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>


#ifndef INCLUDED_base_h_

#define INCLUDED_base_h_

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

#endif
