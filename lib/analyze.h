#include "base.h"


int AnalyzeEtherPacket(Packet *packet, RawPacket *lest_packet);
int AnalyzeIPPacket(Packet *packet, RawPacket *lest_packet);
int AnalyzeUDPPacket(Packet *packet, RawPacket *lest_packet);
int AnalyzeTCPPacket(Packet *packet, RawPacket *lest_packet);
int AnalyzePacket(Packet *packet);