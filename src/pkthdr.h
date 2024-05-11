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

/* mac layer */
typedef struct aka_ethhdr_s {
	uint8_t dst[6];
	uint8_t src[6];
	uint16_t proto; /* upper layer protocols */
} aka_ethhdr_t;

/* ip layer */
#define AKA_IPV6_SIZE 16

typedef struct aka_ipv4hdr_s {
	uint8_t vhl; /* version:4 + 4:header length(* 4) */
	uint8_t tos; /* differentiated services field */

	uint16_t len; /* Total Length(hdr + data) */
	uint16_t id;
	uint16_t frag; /* 1:UNUSED + 1:DF + 1:MF + 13:offset(* 8) */

	uint8_t ttl; /* time to live */
	uint8_t proto; /* upper layer protocols */

	uint16_t hdr_checksum; /* header checksum */
	uint32_t src;
	uint32_t dst;
} aka_ipv4hdr_t;

typedef struct aka_ipv6hdr_s {
    uint32_t vtf; /* version:4 + Traffic Class:8 + Flow Label:20 */
    uint16_t len; /* Payload Length */
    uint8_t nxthdr; /* Next Header */
    uint8_t hop; /* Hop Limit */
    uint8_t src[AKA_IPV6_SIZE];
	uint8_t dst[AKA_IPV6_SIZE];
} aka_ipv6hdr_t;

/* tcp layer */
typedef struct aka_tcphdr_s {
	uint16_t src_port;
	uint16_t dst_port;

	uint32_t seq; /* sequence number */
	uint32_t ack; /* acknowledgment number */

	uint8_t hdr_len; /* header length:4 + reserved flag:3 + accurate ECN:1 */
	uint8_t flags;  /* Congestion Window Reduced|ECN-Echo|urg|ack|push|rst|syn|fin */

	uint16_t window;
	uint16_t checksum;
	uint16_t urp; /* Urgent pointer */
} aka_tcphdr_t;

/* error type */
typedef enum {
	AKA_EPARSE_UNKNOWN = 0,
	AKA_EPARSE_MAC = 1,
	AKA_EPARSE_IP  = 2,
	AKA_EPARSE_TCP = 3,
	AKA_EPARSE_OK  = 4,
} aka_eparse_t;

typedef struct aka_pkt_hdr_s {
	uint64_t no;
    unsigned int ts_sec;
    unsigned int ts_usec;

	uint16_t l3_proto; /* layer 3 protocol */
	union {
		struct {
			uint32_t src;
			uint32_t dst;
		} ipv4;
		struct {
			uint8_t src[16];
			uint8_t dst[16];
		} ipv6;
	} l3;
	uint16_t l4_proto; /* layer 4 protocol */
	uint16_t src_port;
	uint16_t dst_port;
	union {
		struct {

		} udp;
		struct {

		} tcp;
	} l4;

	aka_eparse_t parse_res; /* parse result */
} aka_pkt_hdr_t;

#endif
