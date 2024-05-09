/**
 * @file aka-config.h
 * @author rekord (wildrekord@gmail.com)
 * @date 2024-05-06 13:15
 * 
 * @brief parse conf file
 */

#ifndef AKA_CONFIG_H
#define AKA_CONFIG_H

#define MAX_GROUP_NAME_LENGTH 48

#define MAX_KEY_LENGTH 80
#define MAX_VALUE_LENGTH 80
typedef struct aka_conf_entry_s {
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
    struct aka_conf_entry_s *next;
} aka_conf_entry_t;

typedef struct aka_conf_group_s {
    char name[MAX_GROUP_NAME_LENGTH];
    aka_conf_entry_t *entry;
    struct aka_conf_group_s *next;
} aka_conf_group_t;

/**
 * @brief Create aka_conf_group_t by file name
 * 
 * @param filename 
 * @return corresponding aka_conf_group_t object
 */
aka_conf_group_t *aka_conf_create(const char *filename);

/**
 * @brief destroy aka_conf_group_t object
 * 
 * @param conf the aka_conf_group_t object which will be destroyed
 */
void aka_conf_destroy(aka_conf_group_t *conf);

/**
 * @brief Get configuration value by key
 * 
 * @param p_group point to aka_conf_group_t pointer
 * @param o_key
 * @return the value of corresponding key or NULL if not found
 */
char *aka_conf_get(aka_conf_group_t **p_group, const char *o_key);

#endif
