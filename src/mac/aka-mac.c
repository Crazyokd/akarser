/**
 * @file aka-mac.c
 * @author Rekord (wildrekord@gmail.com)
 * @date 2024-05-11 09:09
 * 
 * @brief 
 */

#include "aka-mac.h"
#include "aka-log.h"
#include "ip/aka-ip.h"

#include <stdio.h>

int aka_decode_eth(aka_pkt_hdr_t *pkt, unsigned char *buf, size_t size)
{
    uint32_t offset = sizeof(aka_ethhdr_t);

    if (offset > size) return -1;
    aka_ethhdr_t *ethhdr = (aka_ethhdr_t *)buf;

    switch (ethhdr->proto) {
    case AKA_IDETH_IPV4:
        pkt->l3_proto = AKA_IPV4;
        break;
    case AKA_IDETH_IPV6:
        pkt->l3_proto = AKA_IPV6;
        break;
    default:
        aka_warn("unknown eth type: %d", ethhdr->proto);
        return -1;
    }
    return offset;
}
