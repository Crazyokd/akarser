/**
 * akarser.c parse pcap file
*/

#include "akarser.h"
#include "aka-log.h"
#include "pkthdr.h"
#include "mac/aka-mac.h"
#include "ip/aka-ip.h"
#include "tcp/aka-tcp.h"

#include <stdint.h>
#include <string.h>

#define RAND_DISCARD 0
static uint8_t use_pcap_ts = 0;

static void aka_parse_data(aka_pkt_hdr_t *pkt, unsigned char *buf, size_t size)
{
    if (!pkt || !buf || !size) return;

    int idx = 0;

    idx += aka_decode_eth(pkt, buf + idx, size);
    if (idx < 0) {
        pkt->parse_res = AKA_EPARSE_MAC;
        return;
    }

    idx += aka_decode_ip(pkt, buf + idx, size);
    if (idx < 0) {
        pkt->parse_res = AKA_EPARSE_TCP;
        return;
    }

    pkt->parse_res = AKA_EPARSE_OK;
}

int aka_parse(unsigned char *buf, size_t size)
{
    unsigned int i = 0;
    pcap_pkthdr_t pcap_pkthdr;
    uint64_t idx = 0;

    aka_pkt_hdr_t *aka_pkthdr = aka_malloc(sizeof(aka_pkt_hdr_t));
    aka_assert(aka_pkthdr);

    idx += sizeof(pcap_file_header_t); // read pcap file header
    do {
        ++i;
        memcpy(&pcap_pkthdr, buf + idx, sizeof(pcap_pkthdr_t));
        idx += sizeof(pcap_pkthdr_t); // read pcap packet header

        memset(aka_pkthdr, 0, sizeof(aka_pkt_hdr_t));
        if (use_pcap_ts) {
            aka_pkthdr->ts_sec = pcap_pkthdr.sec;
            aka_pkthdr->ts_usec = pcap_pkthdr.usec;
        }

#if (RAND_DISCARD == 1)
        if (rand() < (RAND_MAX >> 1))
            aka_parse_data(aka_pkthdr, buf + idx, pcap_pkthdr.len);
#else
        aka_parse_data(aka_pkthdr, buf + idx, pcap_pkthdr.len);
#endif
        idx += pcap_pkthdr.len;
    } while(idx < size);

    free(aka_pkthdr);
    return 0;
}
