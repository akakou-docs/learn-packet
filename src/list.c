void *InitSegment(Segment *segment) {
    *segment = (Segment) {
        NULL,
        NULL
    };
}

void *AddSegment(Segment *segment, Packet *packet) {
    ElementOfSegment *element;
    element = (ElementOfSegment *)malloc(sizeof(ElementOfSegment));
    element -> packet = packet;

    if (segment -> start == NULL || segment -> end == NULL) {
        element -> fd = NULL;
        element -> bk = NULL;

        segment -> start = element;
    }
    else {
        element -> bk = segment -> end;

        ElementOfSegment *one_back = segment -> end;
        one_back -> fd = element;
    }

    segment -> end = element;

    return element;
}

struct IterSegment {
    Segment *segment;
    ElementOfSegment *element;
    int has_next;
} IterSegment;

void InitIterater(Segment *segment) {
    IterSegment = (struct IterSegment){
        segment,
        segment -> start,
        1
    };
}

ElementOfSegment *Iterate() {
    ElementOfSegment *element;
    if (!IterSegment.has_next) {
        return NULL;
    }

    element = IterSegment.element;

    if (IterSegment.segment -> end == element) {
        IterSegment.has_next = 0;
    }
    else {
        IterSegment.element = IterSegment.element -> fd;
    }

    return element;
}

void FreeSegment(Segment *segment) {
    InitIterater(segment);
    while(1) {
        ElementOfSegment *element = Iterate();

        if (element == NULL) {
            break;
        }

        free(element);
    }
}