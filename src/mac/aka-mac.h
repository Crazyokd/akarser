/**
 * @file aka-mac.h
 * @author Rekord (wildrekord@gmail.com)
 * @date 2024-05-11 07:45
 * 
 * @brief 
 */

#ifndef AKA_MAC_H
#define AKA_MAC_H

#include "pkthdr.h"

#include <stddef.h>

#define AKA_IDETH_IPV4            0x0008
#define AKA_IDETH_ARP             0x0608
#define AKA_IDETH_IPV6            0xDD86
#define AKA_IDETH_REVARP          0x3580
#define AKA_IDETH_EAPOL           0x8e88
#define AKA_IDETH_IPX             0x3781
#define AKA_IDETH_PPPoE_DISC      0x6388
#define AKA_IDETH_PPPoE_SESS      0x6488
#define AKA_IDETH_8021Q           0x0081
#define AKA_IDETH_8021AD          0xa888
#define AKA_IDETH_QINQ_NS1        0x0091
#define AKA_IDETH_QINQ_NS2        0x0092
#define AKA_IDETH_LOOP            0x0090
#define AKA_IDETH_MPLS_UNICAST    0x4788
#define AKA_IDETH_MPLS_MULTICAST  0x4888
#define AKA_IDETH_ERSPAN_TYPE2    0xbe88
#define AKA_IDETH_ERSPAN_TYPE3    0xeb22
#define AKA_IDETH_FPATH           0x0389
#define AKA_IDETH_CISCO_META      0x0989
#define AKA_IDETH_LLDP            0xcc88

int aka_decode_eth(aka_pkt_hdr_t *pkt, unsigned char *buf, size_t size);

#endif
