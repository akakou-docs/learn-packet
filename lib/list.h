#include "base.h"


void *InitSegment(Segment *segment);

void *AddSegment(Segment *segment, Packet *packet);

void InitIterater(Segment *segment);

ElementOfSegment *Iterate();

void FreeSegment(Segment *segment);
