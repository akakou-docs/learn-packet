CC              = gcc
OBJS            = server.o
PROGRAM         = server

all:            $(PROGRAM)

$(PROGRAM):     $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(PROGRAM)

debug:          $(OBJS)
	$(CC) -g $(OBJS) $(LDFLAGS) -o $(PROGRAM)

clean:;         rm -f *.o *~ $(PROGRAM)

