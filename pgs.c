/*
 * Copyright (c) 2020 Brenden Davidson
 */

#include "pgs.h"

#include <endian.h>
#include <stdlib.h>
#include <string.h>

uint32_t read_4_bytes(const char *data)
{
    return (uint32_t) (((uint8_t) data[0] << 24u) | ((uint8_t) data[1] << 16u) | ((uint8_t) data[2] << 8u) |
                       ((uint8_t) data[3]));
}

uint16_t read_2_bytes(const char *data)
{
    return (uint16_t) (((uint8_t) data[0] << 8u) | ((uint8_t) data[1]));
}

PgsPresentationComposition *pgs_create_presentation_composition(const char *data, uint16_t size)
{
    PgsPresentationComposition *pcs = NULL;

    if (!data)
    {
        // data is empty, so return a NULL;
        goto ERR;
    }

    if (size < PGS_MIN_PCS_SIZE)
    {
        // data too short to satisfy struct
        goto ERR;
    }

    pcs = malloc(sizeof(PgsPresentationComposition));
    pcs->composition_objects = NULL;

    uint16_t read_pos = 0u;
    pcs->width = read_2_bytes(data + read_pos);
    read_pos += 2;
    pcs->height = read_2_bytes(data + read_pos);
    read_pos += 2;
    pcs->frame_rate = (uint8_t) (data[read_pos]);
    ++read_pos;
    pcs->composition_number = read_2_bytes(data + read_pos);
    read_pos += 2;
    pcs->composition_state = (enum PgsCompositionState) (data[read_pos]);
    ++read_pos;
    pcs->palette_update_flag = (uint8_t) (data[read_pos]);
    ++read_pos;
    pcs->palette_id = (uint8_t) (data[read_pos]);
    ++read_pos;
    pcs->composition_object_count = (uint8_t) (data[read_pos]);
    ++read_pos;

    const uint16_t remaining_size = size - read_pos;
    if (remaining_size < PGS_MIN_PCS_OBJ_SIZE * pcs->composition_object_count)
    {
        // Not enough remaining data to satisfy object struct array.
        goto ERR;
    }

    pcs->composition_objects = malloc(sizeof(void*) * pcs->composition_object_count);
    for (uint8_t i = 0u; i < pcs->composition_object_count; ++i)
    {
        pcs->composition_objects[i] = calloc(1, sizeof(PgsCompositionObject));

        pcs->composition_objects[i]->object_id = read_2_bytes(data + read_pos);
        read_pos += 2;
        pcs->composition_objects[i]->window_id = (uint8_t) (data[read_pos]);
        ++read_pos;
        pcs->composition_objects[i]->cropped_flag = (uint8_t) (data[read_pos]);
        ++read_pos;
        pcs->composition_objects[i]->h_pos = read_2_bytes(data + read_pos);
        read_pos += 2;
        pcs->composition_objects[i]->v_pos = read_2_bytes(data + read_pos);
        read_pos += 2;

        // Some values are only present if the cropped_flag is set.
        if (pcs->composition_objects[i]->cropped_flag == 0x40)
        {
            pcs->composition_objects[i]->crop_h_pos = read_2_bytes(data + read_pos);
            read_pos += 2;
            pcs->composition_objects[i]->crop_v_pos = read_2_bytes(data + read_pos);
            read_pos += 2;
            pcs->composition_objects[i]->crop_width = read_2_bytes(data + read_pos);
            read_pos += 2;
            pcs->composition_objects[i]->crop_height = read_2_bytes(data + read_pos);
            read_pos += 2;
        }
    }

    return pcs;

    ERR:
    // Cleanup pcs on error and return NULL.
    if (pcs)
    {
        pgs_free_presentation_composition(pcs);
    }
    return NULL;
}

void pgs_free_presentation_composition(PgsPresentationComposition *pcs)
{
    if (!pcs)
    {
        // Skip everything if pcs is NULL.
        return;
    }

    if (pcs->composition_objects)
    {
        for (uint8_t i = 0u; i < pcs->composition_object_count; ++i)
        {
            if (pcs->composition_objects[i])
            {
                free(pcs->composition_objects[i]);
                pcs->composition_objects[i] = NULL;
            }
        }
        free(pcs->composition_objects);
    }

    free(pcs);
    pcs = NULL;
}

PgsSegment *pgs_create_segment(const char *data, uint16_t size)
{
    PgsSegment *segment = NULL;

    if (!data)
    {
        goto ERR;
    }

    if (size < PGS_MIN_SEGMENT_SIZE)
    {
        goto ERR;
    }

    segment = malloc(sizeof(PgsSegment));
    uint16_t read_pos = 0u;
    strncpy(segment->magic_number, data + read_pos, 2);
    read_pos += 2;
    segment->presentation_timestamp = read_4_bytes(data + read_pos);
    read_pos += 4;
    segment->decoding_timestamp = read_4_bytes(data + read_pos);
    read_pos += 4;
    segment->segment_type = (enum PgsSegmentType) (data[read_pos]);
    ++read_pos;
    segment->segment_size = read_2_bytes(data + read_pos);
    read_pos += 2;

    const uint16_t remaining_size = size - read_pos;
    switch (segment->segment_type)
    {
        case PresentationComposition:
            segment->data.pcs = pgs_create_presentation_composition(data + read_pos, remaining_size);
            break;
        default:
            break;
    }

    return segment;

    ERR:
        if (segment)
        {
            pgs_free_segment(segment);
        }
        return NULL;
}

void pgs_free_segment(PgsSegment *segment)
{
    if (!segment)
    {
        return;
    }

    switch (segment->segment_type)
    {
        case PresentationComposition:
            if (segment->data.pcs)
            {
                pgs_free_presentation_composition(segment->data.pcs);
            }
            break;
        default:
            break;
    }

    free(segment);
    segment = NULL;
}
