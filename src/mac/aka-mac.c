/**
 * @file aka-mac.c
 * @author Rekord (wildrekord@gmail.com)
 * @date 2024-05-11 09:09
 * 
 * @brief 
 */

#include "aka-mac.h"
#include "pkthdr.h"


#include <stdio.h>

int aka_decode_eth(unsigned char *buf, size_t size)
{
    int offset = sizeof(aka_ethhdr_t);

    if (offset > size) return -1;
    aka_ethhdr_t *ethhdr = (aka_ethhdr_t *)buf;

    switch (ethhdr->proto) {
    case AKA_IDETH_ARP:
        break;
    case AKA_IDETH_IPV4:
    case AKA_IDETH_IPV6:
        break;
    }
    return offset;
}
