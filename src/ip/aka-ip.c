/**
 * @file aka-ip.c
 * @author rekord (wildrekord@gmail.com)
 * @date 2024-05-11 13:33
 * 
 * @brief 
 */

#include "aka-ip.h"
#include "aka-log.h"

#include <arpa/inet.h>
#include <string.h>

int aka_decode_ip(aka_pkt_hdr_t *pkt, unsigned char *buf, size_t size)
{
    if (!pkt || !buf || !size) return -1;

    int offset = 0;

    if (pkt->l3_proto == AKA_IPV4) {
        aka_ipv4hdr_t *ipv4hdr = (aka_ipv4hdr_t *)buf;
        pkt->l4_proto = ipv4hdr->proto;
        pkt->l3.ipv4.src = ipv4hdr->src;
        pkt->l3.ipv4.dst = ipv4hdr->dst;
    } else if (pkt->l3_proto == AKA_IPV6) {
        aka_ipv6hdr_t *ipv6hdr = (aka_ipv6hdr_t *)buf;
        pkt->l4_proto = ipv6hdr->nxthdr;
        memcpy(pkt->l3.ipv6.src, ipv6hdr->src, AKA_IPV6_SIZE);
        memcpy(pkt->l3.ipv6.dst, ipv6hdr->dst, AKA_IPV6_SIZE);
    } else {
        aka_warn("unknown l3_proto: %d", pkt->l3_proto);
        return -1;
    }

    return offset;
}
