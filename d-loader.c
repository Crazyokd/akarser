/**
 * @file d-loader.c
 * @author rekord (wildrekord@gmail.com)
 * @date 2024-05-07 13:40
 * 
 * @brief load and register dynamic libraries
 */

#include "d-loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

typedef struct aka_decoder_s {
    protocol_id_t proto_id;
    decoder_init_func init_fn;
    decoder_process_func process_fn;
    decoder_destroy_func destroy_fn;
} aka_decoder_t;

aka_decoder_t aka_decoders[MAX_PROTO] = {0};

static char *aka_str_split(char *str, char *dst, char delimiter)
{
    if (!str) return NULL;

    char *end = strchr(str, delimiter);
    char *next_start = NULL;
    if (end) {
        next_start = end + 1;
    } else {
        end = str + strlen(str);
    }
    strncpy(dst, str, end - str);
    dst[end - str] = 0;

    return next_start;
}

#define SEG_CNT 4

static int aka_register_decoder(int proto_id, char *prefix, const char *name, const char *conf)
{
    void *handle = NULL;

    if (proto_id >= MAX_PROTO || proto_id <= UNKNOWN_PROTO || !name) {
        return -1;
    }

    if (aka_decoders[proto_id].proto_id) {
        /* decoder is already loaded */
        return -1;
    }
    char lib_path[256];
    if (strlen(prefix)) {
        snprintf(lib_path, sizeof(lib_path), "%s%s", prefix, name);
    } else {
        snprintf(lib_path, sizeof(lib_path), "./%s", name);
    }

    handle = dlopen(lib_path, RTLD_LAZY);
    if (!handle) {
        printf("open %s fail!\n", name);
        return -1;
    }

    aka_decoders[proto_id].process_fn = (decoder_process_func)dlsym(
        handle, "aka_decoder_process");
    if (!aka_decoders[proto_id].process_fn) {
        printf("load %s fail, no \"aka_decoder_process\" function in lib!\n", name);
        dlclose(handle);
        return -1;
    }
    aka_decoders[proto_id].init_fn = (decoder_init_func)dlsym(
        handle, "aka_decoder_init");
    aka_decoders[proto_id].destroy_fn = (decoder_destroy_func)dlsym(
        handle, "aka_decoder_destroy");

    if (aka_decoders[proto_id].init_fn && aka_decoders[proto_id].init_fn(conf)) {
        printf("init %s fail!\n", name);
        dlclose(handle);
        return -1;
    }
    /* load dynamic library successfully */
    aka_decoders[proto_id].proto_id = proto_id;
    dlclose(handle);

    return 0;
}

/**
 * @brief parse decoder conf
 * 
 * @param str 
 * @return 1 if success or 0 if failed
 */
static int aka_parse_decoder_conf(char *str)
{
    if (!str) return 0;

    char *tok = str;

    int idx = 0;
    char segs[SEG_CNT][MAX_VALUE_LENGTH] = {0};
    while (tok) {
        tok = aka_str_split(tok, segs[idx++], ',');
    }

    if (!aka_register_decoder(atoi(segs[0]), segs[1], segs[2], segs[3])) {
        printf("loaded decoder[%d], prefix: \"%s\", name: \"%s\", conf: \"%s\"\n",
               atoi(segs[0]), segs[1], segs[2], segs[3]);
        return 1;
    }
    return 0;
}

int aka_load_decoders(aka_conf_group_t *group)
{
    if (!group) return -1;

    int ret = 0;
    aka_conf_group_t *group_sp = group; /* record start point */
    aka_conf_entry_t *entry;
    do {
        if (!strcmp(group->name, "decoders")) {
            entry = group->entry;
            while (entry) {
                ret += aka_parse_decoder_conf(entry->value);
                entry = entry->next;
            }
        }
        group = group->next;
    } while (group != group_sp);

    return ret;
}