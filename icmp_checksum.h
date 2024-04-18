// Patryk Maciąg 331542
#ifndef ICMP_CHECKSUM_H
#define ICMP_CHECKSUM_H
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

u_int16_t compute_icmp_checksum (const void *buff, int length);

#endif
