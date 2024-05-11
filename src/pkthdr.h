/**
 * @file pkt-header.h
 * @author Rekord (wildrekord@gmail.com)
 * @date 2024-05-11 07:23
 * 
 * @brief 
 */

#ifndef AKA_PKT_HEADER_H
#define AKA_PKT_HEADER_H

#include <stdint.h>

/* extract from pcap/pcap.h */
typedef struct pcap_file_header_s {
	uint32_t magic;
	uint16_t version_major;
	uint16_t version_minor;
	int32_t thiszone;	/* gmt to local correction; this is always 0 */
	uint32_t sigfigs;	/* accuracy of timestamps; this is always 0 */
	uint32_t snaplen;	/* max length saved portion of each pkt */
	uint32_t linktype;	/* data link type (LINKTYPE_*) */
} pcap_file_header_t;

typedef struct pcap_pkthdr_s {
	uint32_t sec;
	uint32_t usec;
	uint32_t caplen;	/* length of portion present */
	uint32_t len;	/* length of this packet (off wire) */
} pcap_pkthdr_t;

typedef struct aka_pkt_hdr_s {
    unsigned int ts_sec;
    unsigned int ts_usec;

	uint8_t parse_res; /* parse result */
} aka_pkt_hdr_t;

#endif
