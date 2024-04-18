// Patryk Maciąg 331542
#ifndef ICMP_RECEIVE_H
#define ICMP_RECEIVE_H
#include "utils.h"

void print_as_bytes(unsigned char* buff, ssize_t length);
void icmp_receive(int sockfd, int ttl, int pid, int *packets, int *final_ip, char (*ip)[20]);

#endif
