#include <stdio.h>
#include <stdint.h>

#include "akarser.h"
#include "config.h"
#include "d-loader.h"
#include "aka-core.h"

int main()
{
    /* parse main configuration file */
    aka_conf_group_t *group = aka_conf_create("akarser.conf");
    if (!group) {
        return -1;
    }

    /* load and register dynamic libraries */
    aka_load_decoders(group);

    /* read and parse pcap file */
    FILE *file = fopen(aka_conf_get(&group, "pcap_file"), "rb");
    if (!file) {
        perror("Error opening file");
        aka_conf_destroy(group);
        return -1;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the array
    unsigned char *array = (unsigned char *)aka_malloc(file_size);
    if (!array) {
        perror("Error allocating memory");
        fclose(file);
        aka_conf_destroy(group);
        return -1;
    }

    // Read the file contents into the array
    size_t bytes_read = fread(array, 1, file_size, file);
    if (bytes_read != file_size) {
        perror("Error reading file");
        aka_free(array);
        fclose(file);
        aka_conf_destroy(group);
        return -1;
    }

    uint32_t repeat_cnt = atoi(aka_conf_get(&group, "repeat_cnt"));
    while (repeat_cnt--) {
        aka_parse(array, file_size);
    }

    // Close the file and free the memory
    aka_free(array);
    fclose(file);
    aka_conf_destroy(group);
    return 0;
}