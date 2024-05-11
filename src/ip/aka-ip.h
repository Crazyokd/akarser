/**
 * @file aka-ip.h
 * @author rekord (wildrekord@gmail.com)
 * @date 2024-05-11 13:32
 * 
 * @brief 
 */

#ifndef AKA_IP_H
#define AKA_IP_H

#include "pkthdr.h"

#include <stddef.h>

int aka_decode_ip(aka_pkt_hdr_t *pkt, unsigned char *buf, size_t size);

#define AKA_IPV4 4
#define AKA_IPV6 6

#endif
