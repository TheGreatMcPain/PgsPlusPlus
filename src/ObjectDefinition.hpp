//
// Created by bdavidson on 10/24/20.
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
     * \brief Denotes the in-sequence ordering of object data.
     */
    enum class SequenceFlag
    {
        Last = 0x40, /**< Used when object data is last in its sequence */
        First = 0x80, /**< Used when object data is first in its sequence */
        Only = 0xC0 /**< Used when there is only one object data array in sequence */
    };

    /**
     * \brief Subclass of SegmentData containing information needed for constructing a subtitle
     * image.
     */
    class ObjectDefinition : public SegmentData
    {
    protected:
        /**
         * \brief Reads 3 bytes of data from the provided data.
         * \details
         * The number of bytes used to store an object data segment is held as a 24-bit value in the PGS file.
         * A 32-bit value is the next closest thing we can use on an average computer, so the value is cast to 32-bits
         * and 0-padded.
         *
         * \param data pointer to raw data array
         * \param readPos position in data array to read from
         * \return 32-bit value containing read data.
         */
        inline static uint32_t read3Bytes(const uint8_t *data, uint16_t &readPos);
    public:
        static constexpr uint16_t MIN_BYTE_SIZE = 11u;

        uint16_t id; /**< ID of this object */
        uint8_t version; /**< Version of this object */
        SequenceFlag sequenceFlag; /**< Order of this object in its sequence */
        uint32_t dataLength; /**< Number of bytes contained in the object data vector. */
        uint16_t width; /**< Width of image after decompression */
        uint16_t height; /**< Height of image after decompression */
        vector<uint8_t> objectData; /**< RLE-compressed object data. */

        /**
         * \brief Creates a new ObjectDefinition instance.
         */
        ObjectDefinition();

        void import(const char *data, const uint16_t &size) override;

        void import(const vector<char> &data) override;
    };
}
