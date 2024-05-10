/**
 * @file aka-config.c
 * @author Rekord (wildrekord@gmail.com)
 * @date 2024-05-07 02:08
 * 
 * @brief 
 */

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "aka-config.h"
#include "aka-core.h"

#define MAX_LINE_LENGTH 1024
const char *default_group_name = "global";

/**
 * @brief trim space and remove comment for str
 * 
 * @param str 
 * @return 
 */
static char *strtrim(char *str)
{
    if (!str) return NULL;

    char *end = str + strlen(str);

    /* remove prefix space */
    while (str != end && isspace(*str)) str++;
    if (str == end) return NULL;

    /* 现在str至少含有一个有效字符 */
    /* remove suffix space */
    end--;
    while (end > str && isspace(*end)) end--;
    ++end;

    /* now start removing comment */
    char *cursor = str;
    while (cursor != end && *cursor != '#') cursor++;
    if (cursor != end) {
        /* 找到了第一个# */
        end = cursor;
    }
    *end = 0;
    if (str == end) return NULL;
    return str;
}

static aka_conf_group_t *aka_conf_group_create(const char *name)
{
    if (!name) return NULL;
    aka_conf_group_t *group = aka_malloc(sizeof(aka_conf_group_t));
    if (!group) return NULL;

    memset(group, 0, sizeof(aka_conf_group_t));
    strncpy(group->name, name, MAX_GROUP_NAME_LENGTH);
    return group;
}

static aka_conf_entry_t *aka_conf_entry_create(aka_conf_group_t *group, const char *key, const char *value)
{
    if (!group || !key || !value || !strlen(key) || !strlen(value)) return NULL;

    aka_conf_entry_t *entry = aka_malloc(sizeof(aka_conf_entry_t));
    if (!entry) return NULL;

    strncpy(entry->key, key, MAX_KEY_LENGTH);
    strncpy(entry->value, value, MAX_VALUE_LENGTH);
    entry->next = group->entry;
    group->entry = entry;
    return entry;
}

/**
 * @brief find group by name or create group as needed
 * 
 * @param group 
 * @param name 
 * @param len 
 * @return Found group or newly created group
 */
aka_inline static aka_conf_group_t *aka_conf_parse_group_name(aka_conf_group_t *group, const char *name)
{
    if (!group || !name) return NULL;

    aka_conf_group_t *group_sp = group; /* record start point */
    do {
        /* todo: 是否需要区分大小写 */
        if (strcmp(name, group->name) == 0) {
            return group;
        }
        group = group->next;
    } while (group != group_sp);

    aka_conf_group_t *new_group = aka_conf_group_create(name);
    if (!new_group) return NULL;

    new_group->next = group->next;
    group->next = new_group;
    return new_group;
}

/**
 * @brief parse key-value pair
 * 
 * @param group 
 * @param line 
 */
aka_inline static void aka_conf_parse_pair(aka_conf_group_t *group, const char *line)
{
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
    sscanf(line, "%[^=]=%s", key, value);
    // trim suffix of key and prefix of value
    int key_len = strlen(key) - 1;
    while (key_len >= 0 && isspace(key[key_len])) key_len--;
    key[key_len + 1] = 0;

    char *value_end = value + strlen(value);
    char *value_cursor = value;
    while (value_cursor != value_end && isspace(*value_cursor)) value_cursor++;

    aka_conf_entry_create(group, key, value_cursor);
}

aka_conf_group_t *aka_conf_create(const char *conf_file)
{
    char line[MAX_LINE_LENGTH];
    char *p_line; /* processd line */
    FILE *file = fopen(conf_file, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    aka_conf_group_t *group = aka_conf_group_create(default_group_name);
    if (!group) {
        goto end;
    }
    /* 第一个group的下一个group指向其本身 */
    group->next = group;

    while (fgets(line, sizeof(line), file) != NULL) {
        p_line = strtrim(strtrim(line));
        if (!p_line) continue;
        if (p_line[0] == '[' && p_line[strlen(p_line) - 1] == ']') {
            p_line[strlen(p_line) - 1] = 0;
            group = aka_conf_parse_group_name(group, p_line + 1);
        } else {
            aka_conf_parse_pair(group, p_line);
        }
    }
end:
    fclose(file);
    return group;
}

void aka_conf_destroy(aka_conf_group_t *group)
{
    aka_conf_group_t *group_sp = group; /* record start point */
    aka_conf_entry_t *next;
    aka_conf_group_t *group_next;
    do {
        while (group->entry) {
            next = group->entry->next;
            aka_free(group->entry);
            group->entry = next;
        }
        group_next = group->next;
        aka_free(group);
        group = group_next;
    } while (group != group_sp);
}

char *aka_conf_get(aka_conf_group_t **p_group, const char *o_key)
{
    if (!p_group || !*p_group || !o_key || !strlen(o_key)) return NULL;

    aka_conf_group_t *group = *p_group;
    aka_conf_entry_t *entry;
    int i = 0;
    int o_key_len = strlen(o_key);
    char group_name[MAX_KEY_LENGTH] = {0};
    char key[MAX_KEY_LENGTH] = {0};
    int have_group_name = 0;
    /* get group name */
    while (i < o_key_len && o_key[i]) {
        if (key[i] == '.') {
            strncpy(group_name, o_key, i);
            group_name[i] = 0;
            strncpy(key, o_key + i + 1, o_key_len - i - 1);
            key[o_key_len - i - 1] = 0;
            have_group_name = 1;
            break;
        }
        i++;
    }

    if (!have_group_name) {
        strncpy(group_name, default_group_name, strlen(default_group_name) + 1);
        strncpy(key, o_key, o_key_len);
    }
    if (!strlen(key)) {
        return NULL;
    }

    do {
        if (strcmp(group_name, group->name) == 0) {
            entry = group->entry;
            while (entry) {
                if (strcmp(key, entry->key) == 0) {
                    /* perf: 利用局部性原理 */
                    *p_group = group;
                    return entry->value;
                }
                entry = entry->next;
            }
        }
        group = group->next;
    } while (group != *p_group);

    return NULL;
}

aka_conf_entry_t *aka_conf_get_entry(aka_conf_group_t *group, const char *name)
{
    if (!group || !name || !strlen(name)) return NULL;

    aka_conf_group_t *group_sp = group; /* record start point */
    do {
        if (strcmp(name, group->name) == 0) {
            return group->entry;
        }
        group = group->next;
    } while (group != group_sp);

    return NULL;
}
