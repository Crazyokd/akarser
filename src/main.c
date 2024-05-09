#include <stdio.h>
#include <stdint.h>

#include "akarser.h"
#include "aka-config.h"
#include "dloader.h"
#include "aka-log.h"

int main()
{
    /* parse main configuration file */
    aka_conf_group_t *group = aka_conf_create("akarser.conf");
    aka_assert(group);

    /* load and register dynamic libraries */
    aka_load_decoders(group);

    /* read and parse pcap file */
    FILE *file = fopen(aka_conf_get(&group, "pcap_file"), "rb");
    aka_assert(file);

    // Determine the file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the array
    unsigned char *array = (unsigned char *)aka_malloc(file_size);
    aka_assert(array);

    // Read the file contents into the array
    size_t bytes_read = fread(array, 1, file_size, file);
    aka_assert(bytes_read == file_size);
    fclose(file);

    uint32_t repeat_cnt = atoi(aka_conf_get(&group, "repeat_cnt"));
    while (repeat_cnt--) {
        aka_parse(array, file_size);
    }

    // Close the file and free the memory
    aka_free(array);
    aka_conf_destroy(group);
    return 0;
}