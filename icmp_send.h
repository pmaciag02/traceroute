// Patryk Maciąg 331542
#ifndef ICMP_SEND_H
#define ICMP_SEND_H
#include "utils.h"

void icmp_send(int sockfd, struct sockaddr_in recipient, int ttl, int pid);

#endif
