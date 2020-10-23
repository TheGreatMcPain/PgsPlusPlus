/*
 * Copyright (c) 2020 Brenden Davidson
 */

#pragma once

#include <stdint.h>

// ============
// Generic data
// ============

#define PGS_MAGIC_NUMBER 0x5047

// ======================================================
// Presentation Composition Structs, Enums, and Functions
// ======================================================

/**
 * \brief Enumeration of possible composition states.
 */
enum PgsCompositionState
{
    Normal = 0x00, /**< Defines a <em>display update</em>. Contains only functional segments that differ from the preceding composition. Commonly used to stop the display of the previous display set. */
    AcquisitionPoint = 0x40, /**< Defines a <em>display refresh</em>. Segment contains data used to update the object with the same ID. */
    EpochStart = 0x80 /**< Defines a <em>new display</em>. Contains all functional segments needed to show a new composition. */
};

static const uint16_t PGS_MIN_PCS_OBJ_SIZE = 8;

/**
 * \brief Individual Composition Object used to define display attributes of the subtitle image.
 */
typedef struct PgsCompositionObject
{
    uint16_t object_id; /**< ID of the associated Object Definition Segment. */
    uint8_t window_id; /**< ID of the associated Window Definition Segment. Up to 2 images can use 1 window. */
    uint8_t cropped_flag; /**< Set to 0x40 to force display of the cropped image object; 0x00, otherwise. */
    uint16_t h_pos; /**< Horizontal (x) offset from the top-left pixel of the video frame. */
    uint16_t v_pos; /**< Vertical (y) offset from the top-left pixel of the video frame. */
    uint16_t crop_h_pos; /**< Horizontal (x) crop offset from the top-left pixel of the video frame. */
    uint16_t crop_v_pos; /**< Vertical (y) crop offset from the top-left pixel of the video frame. */
    uint16_t crop_width; /**< Width of the crop. */
    uint16_t crop_height; /**< Height of the crop. */
} PgsCompositionObject;

static const uint16_t PGS_MIN_PCS_SIZE = 11u;

/**
 * \brief Presentation Composition structure containing all relevant display info for the subtitle image.
 */
typedef struct PgsPresentationComposition
{
    uint16_t width;     /**< Pixels in video width. */
    uint16_t height;    /**< Pixels in video height */
    uint8_t frame_rate; /**< Subtitle display frame rate. Should almost always be 0x10 (24fps) */
    uint16_t composition_number; /**< Index of this composition. This value is incremented by one for each graphics update. */
    enum PgsCompositionState composition_state; /**< Type of composition state being defined. */
    uint8_t palette_update_flag; /**< Set to 0x80 if this segment describes a <em>palette only</em> update; 0x00, otherwise. */
    uint8_t palette_id; /**< ID of palette to use in palette-only update. */
    uint8_t composition_object_count; /**< Number of composition objects defined in segment. */
    PgsCompositionObject **composition_objects; /**< Array of Composition Objects in this segment. */
} PgsPresentationComposition;

/**
 * \brief Creates a Presentation Composition struct using the provided data.
 *
 * \details
 * A pointer to the newly-created struct is provided if the function was successful; otherwise, a NULL is returned. The
 * user is responsible for freeing the data using pgs_free_presentation_composition.
 *
 * \param data Pointer to data to import
 * \param size Number of bytes in provided data.
 * \return pointer to newly-constructed PgsPresentationComposition.
 */
PgsPresentationComposition *pgs_create_presentation_composition(const char *data, uint16_t size);

/**
 * \brief Frees the data in the provided PgsPresentationComposition struct pointer and sets it to NULL when finished.
 * \param pcs pointer to PgsPresentationComposition struct to free.
 */
void pgs_free_presentation_composition(PgsPresentationComposition *pcs);

// =========================
// Window Definition Structs
// =========================

typedef struct PgsWindowObject
{
    uint8_t id; /**< Window ID */
    uint16_t h_pos; /**< Horizontal (x) offset from the top-left pixel of the video frame. */
    uint16_t v_pos; /**< Vertical (y) offset from the top-left pixel of the video frame. */
    uint16_t width; /**< Window width */
    uint16_t height; /**< Window height */
} PgsWindowObject;

typedef struct PgsWindowDefinition
{
    uint8_t num_windows; /**< Number of Windows contained in this segment. */
    PgsWindowObject **windows; /**< Array of Window Objects in this segment. */
} PgsWindowDefinition;

// ==========================
// Palette Definition Structs
// ==========================

typedef struct PgsPaletteEntry
{
    uint8_t id; /**< Palette Entry ID */
    uint8_t y; /**< Luminance value of the color (0-255) */
    uint8_t cr; /**< Red color difference (0-255) */
    uint8_t cb; /**< Blue color difference (0-255) */
    uint8_t alpha; /**< Transparency (0-255) */
} PgsPaletteEntry;

typedef struct PgsPaletteDefinition
{
    uint8_t id; /**< Palette ID */
    uint8_t version; /**< Version of palette within the epoch. */
    uint8_t num_entries; /**< Number of palette entries. Computed from remaining data in segment. */
    PgsPaletteEntry **entries; /**< Array of Palette Entries in this segment. */
} PgsPaletteDefinition;

// =========================
// Object Definition Structs
// =========================

enum PgsObjectSequence
{
    Last = 0x40,
    First = 0x80,
    Only = 0xC0
};

typedef struct PgsObjectDefinition
{
    uint16_t id; /**< Object ID */
    uint8_t version; /**< Version of the object */
    enum PgsObjectSequence sequenceFlag;
    uint32_t data_size; /**< 3-byte value defining the number of bytes of compressed data. */
    uint16_t width; /**< Image width after decompression */
    uint16_t height; /**< Image height after decompression */
    char *img_data; /**< RLE-compressed image data. Size is defined in data_size field. */
} PgsObjectDefinition;

// =======================
// Core PGS structs & data
// =======================

/**
 * \brief Enumeration of data segment types found in Presentation Graphic Stream data.
 */
enum PgsSegmentType
{
    PaletteDefinition = 0x14,       /**< Segment contains palette information for the display set. */
    ObjectDefinition = 0x15,        /**< Segment contains compressed data defining all or part of a subtitle image. */
    PresentationComposition = 0x16, /**< Segment contains composition information for the display set. */
    WindowDefinition = 0x17,        /**< Segment contains information on the region of the display used for showing the subtitle. */
    EndOfDisplaySet = 0x80          /**< Segment denotes the end of a display set. No information is contained. */
};

static const uint16_t PGS_MIN_SEGMENT_SIZE = 13;

/**
 * \brief Full struct definition of a PGS data segment.
 *
 * \details
 * Each PgsSegment contains some header data (everything from the magic_number to the segment_size) along with a union
 * containing the corresponding structure which defines the segment type.
 */
typedef struct PgsSegment
{
    char magic_number[2];               /**< 2-byte value defining the start of a segment. Should always be PGS_MAGIC_NUMBER */
    uint32_t presentation_timestamp;    /**< 32-bit value defining the presentation time of the segment. */
    uint32_t decoding_timestamp;        /**< 32-bit value defining the decoding time of the segment. */
    enum PgsSegmentType segment_type;   /**< Type of data contained in this segment. */
    uint16_t segment_size;              /**< Number of bytes containing the segment data. */
    union SegmentData
    {
        PgsPresentationComposition *pcs;
        PgsWindowDefinition *wds;
        PgsPaletteDefinition *pds;
        PgsObjectDefinition *ods;
    } data;
} PgsSegment;

PgsSegment *pgs_create_segment(const char *data, uint16_t size);

void pgs_free_segment(PgsSegment *segment);
