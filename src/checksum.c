#include    <stdio.h>
#include    <string.h>
#include    <sys/types.h>
#include    <unistd.h>
#include    <sys/ioctl.h>
#include    <arpa/inet.h>
#include    <sys/socket.h>
#include    <linux/if.h>
#include    <net/ethernet.h>
#include    <netpacket/packet.h>
#include    <netinet/if_ether.h>
#include    <netinet/ip.h>
#include    <netinet/ip6.h>
#include    <netinet/ip_icmp.h>
#include    <netinet/icmp6.h>
#include    <netinet/tcp.h>
#include    <netinet/udp.h>

typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;

// 疑似ヘッダ
struct pseudo_ip{
    struct in_addr ip_src;
    struct in_addr ip_dst;
    unsigned char dummy;
    unsigned char ip_p;
    unsigned short ip_len;
};

// 疑似ヘッダ
struct pseudo_ip6_hdr{
    struct in6_addr src;
    struct in6_addr dst;
    unsigned long plen;
    unsigned short dmy1;
    unsigned char dmy2;
    unsigned char nxt;
};

u_int16_t checksum(unsigned char *data, int len)
{
    // 変数を宣言
    register u_int32_t sum;   // チェックサム
    register u_int16_t *ptr;  // 残りのポインタ
    register int c;           // 

    // チェックサムを0で初期化
    sum = 0;
    // dataをu_int16_tに無理やりキャストして代入
    ptr = (u_int16_t *)data;

    
    // 16ビットごとの1の補数和を取って、
    // さらにその1の補数和をとって返す
    for(c = len; c > 1; c -= 2){
        sum += (*ptr);
        if(sum & 0x80000000){
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
        ptr++;
    }

    if(c == 1){
        u_int16_t val;
        val = 0;
        memcpy(&val, ptr, sizeof(u_int8_t));
        sum += val;
    }

    while(sum >> 16){
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return(~sum);
}

u_int16_t checksum2(unsigned char *data1, int len1, unsigned char *data2, int len2)
{
    // 変数を宣言
    register u_int32_t sum;
    register u_int16_t *ptr;
    register int c;

    // チェックサムを0で初期化
    sum = 0;

    ptr = (u_int16_t *)data1;
    for(c = len1; c > 1; c -= 2){
        sum += (*ptr);
        if(sum & 0x80000000){
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
        ptr ++;
    }
    if(c == 1){
        u_int16_t    val;
        val = ((*ptr) << 8) + (*data2);
        sum += val;
        if(sum & 0x80000000){
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
        ptr = (u_int16_t *)(data2 + 1);
        len2 --;
    }
    else{
        ptr = (u_int16_t *)data2;
    }
    for(c = len2; c > 1; c-= 2){
        sum += (*ptr);
        if(sum & 0x80000000){
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
        ptr ++;
    }
    if(c == 1){
        u_int16_t val;
        val = 0;
        memcpy(&val, ptr, sizeof(u_int8_t));
        sum += val;
    }

    while(sum >> 16){
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return(~sum);
}

int checkIPchecksum(struct iphdr *iphdr, unsigned char *option, int optionLen)
{
unsigned short    sum;

    if(optionLen == 0){
    sum = checksum((unsigned char *)iphdr, sizeof(struct iphdr));
    if(sum == 0 || sum == 0xFFFF){
        return(1);
    }
    else{
        return(0);
    }
    }
    else{
    sum = checksum2((unsigned char *)iphdr, sizeof(struct iphdr), option, optionLen);
    if(sum == 0 || sum == 0xFFFF){
        return(1);
    }
    else{
        return(0);
    }
    }
}

int checkIPDATAchecksum(struct iphdr *iphdr, unsigned char *data, int len)
{
struct pseudo_ip    p_ip;
unsigned short  sum;

    memset(&p_ip, 0, sizeof(struct pseudo_ip));
    p_ip.ip_src.s_addr = iphdr -> saddr;
    p_ip.ip_dst.s_addr = iphdr -> daddr;
    p_ip.ip_p = iphdr -> protocol;
    p_ip.ip_len = htons(len);

    sum = checksum2((unsigned char *)&p_ip, sizeof(struct pseudo_ip), data, len);
    if(sum == 0 || sum == 0xFFFF){
        return(1);
    }
    else{
        return(0);
    }
}

