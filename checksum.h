#include "checksum.c"
#include <stdint.h>

typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
 
u_int16_t checksum(unsigned char *data,int len);
u_int16_t checksum2(unsigned char *data1,int len1,unsigned char *data2,int len2);
int checkIPchecksum(struct iphdr *iphdr, unsigned char *option, int optionLen);
int checkIPDATAchecksum(struct iphdr *iphdr,unsigned char *data,int len);