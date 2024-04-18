# Compiler
CC = gcc

CFLAGS = -std=gnu99 -Wall -Werror -Wextra

SRCS = main.c icmp_send.c icmp_receive.c icmp_checksum.c

OBJS = $(SRCS:.c=.o)

TARGET = traceroute

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main.o: main.c icmp_send.h icmp_receive.h icmp_checksum.h utils.h
icmp_send.o: icmp_send.c icmp_send.h utils.h
icmp_receive.o: icmp_receive.c icmp_receive.h utils.h
icmp_checksum.o: icmp_checksum.c icmp_checksum.h

clean:
	rm -f $(OBJS) $(TARGET)

