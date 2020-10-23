//
// Created by bdavidson on 10/22/20.
//

#include <pgs.h>
#include <stdio.h>
#include <stdlib.h>

int test_create_pgs_segment()
{
    FILE *sup_file = fopen("./res/subs_short.sup", "rb");
    if (!sup_file)
    {
        fprintf(stderr, "Failed to open SUP data file.\n");
    }

    const uint32_t data_size = 32;
    fseek(sup_file, 140, SEEK_SET);

    char *data = calloc(data_size, sizeof(char));
    for(uint32_t i = 0u; i < data_size; ++i)
    {
        data[i] = (char)(fgetc(sup_file));
    }

    fclose(sup_file);

    PgsSegment *segment = pgs_create_segment(data, data_size);
    free(data);

    if (!segment)
    {
        return EXIT_FAILURE;
    }

    pgs_free_segment(segment);
    return EXIT_SUCCESS;
}

int main(void)
{
    uint32_t num_failed = 0;

    int result = test_create_pgs_segment();
    if (result == EXIT_FAILURE)
    {
        ++num_failed;
    }

    return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
