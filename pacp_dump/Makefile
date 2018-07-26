OBJS=pcap.o analyze.o checksum.o print.o
SRCS=$(OBJS:%.o=%.c)
CFLAGS=-g -Wall
LDLIBS=
TARGET=pcap
$(TARGET):$(OBJS)
	$(cc) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)