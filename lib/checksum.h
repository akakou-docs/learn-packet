#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netpacket/packet.h>
#include <netinet/if_ether.h>


u_int16_t checksum(unsigned char *data,int len);
u_int16_t checksum2(unsigned char *data1,int len1,unsigned char *data2,int len2);
int checkIPchecksum(struct iphdr *iphdr, unsigned char *option, int optionLen);
int checkIPDATAchecksum(struct iphdr *iphdr,unsigned char *data,int len);

u_int16_t checksum3(struct iphdr *iphdr,unsigned char *data,int len);