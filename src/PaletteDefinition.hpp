//
// Created by bdavidson on 10/23/20.
//

#pragma once

#include "pgs.hpp"
#include "SegmentData.hpp"
#include <cstdint>
#include <vector>

using std::vector;

namespace Pgs
{
    /**
     * \brief A single color palette used in a PGS subtitle image
     */
    class PaletteEntry
    {
    public:
        /**
         * \brief Minimum number of bytes needed to create a basic PaletteEntry instance.
         */
        static constexpr uint16_t MIN_BYTE_SIZE = 5u;

        uint8_t id = 0u; /**< Palette Entry ID */
        uint8_t y = 0u; /**< Luminance value of the color (0-255) */
        uint8_t cr = 0u; /**< Red color difference (0-255) */
        uint8_t cb = 0u; /**< Blue color difference (0-255) */
        uint8_t alpha = 0u; /**< Transparency (0-255) */

        /**
         * \brief Constructs a new PaletteEntry instance.
         */
        PaletteEntry() = default;

        void import(const char *data, const uint16_t &size, uint16_t &readPos);
    };

    /**
     * \brief SegmentData defining the PaletteEntries in use in a specific subtitle image.
     */
    class PaletteDefinition : public SegmentData
    {
    public:
        /**
         * \brief Minimum number of bytes needed to create a basic PaletteDefinition instance.
         */
        static constexpr uint16_t MIN_BYTE_SIZE = 3u;

        uint8_t id; /**< Palette ID */
        uint8_t version; /**< Version of palette within the epoch. */
        uint8_t numEntries; /**< Number of palette entries. Computed from remaining data in segment. */
        vector<PaletteEntry> entries; /**< Vector of PaletteEntries in this segment */

        /**
         * \brief Constructs a new PaletteDefinition instance.
         */
        PaletteDefinition();

        void import(const char *data, const uint16_t &size) override;

        void import(const vector<char> &data) override;
    };
}
