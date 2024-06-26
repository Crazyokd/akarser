/**
 * @file dloader.h
 * @author rekord (wildrekord@gmail.com)
 * @date 2024-05-07 13:40
 * 
 * @brief load and register dynamic libraries
 */

#ifndef AKA_DLOADER_H
#define AKA_DLOADER_H

#include "aka-config.h"
#include "pkthdr.h"

typedef enum {
    UNKNOWN_PROTO     = 0,
    SCTP_PROTO        = 1,
    GTPV2_PROTO       = 2,
    H323_PROTO        = 3,
    HTTP2_PROTO       = 4,
    MAX_PROTO
} protocol_id_t;

typedef int (*decoder_init_func)(const char *conf);
typedef int (*decoder_process_func)(aka_pkt_hdr_t *hdr);
typedef int (*decoder_destroy_func)();

/**
 * @brief load all decoders by configuration
 * 
 * @param group 
 * @return Number of successfully loaded library
 */
int aka_load_decoders(aka_conf_group_t *group);

/**
 * @brief free all decoders
 * 
 */
void aka_free_decoders();

#endif