/*
 *  libpgs: A C++ library for reading Presentation Graphics Stream (PGS) subtitles.
 *  Copyright (C) 2020  Brenden Davidson
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 * 
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 *  USA
*/

#pragma once

#include "SegmentData.hpp"

#include <memory>
#include <vector>

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
        uint16_t id; /**< ID of this object */
        uint8_t version; /**< Version of this object */
        SequenceFlag sequenceFlag; /**< Order of this object in its sequence */
        uint32_t dataLength; /**< Number of bytes contained in the object data vector. */
        uint16_t width; /**< Width of image after decompression */
        uint16_t height; /**< Height of image after decompression */
        std::vector<uint8_t> objectData; /**< RLE-compressed object data. */

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

        /**
         * \brief Decode an individual line from the object data.
         * \param startPos position to start reading from
         * \param endPos position to stop reading at
         * \return decompressed line
         */
        [[nodiscard]] std::vector<uint8_t> decodeLine(const uint32_t &startPos, const uint32_t &endPos) const;
    public:
        static constexpr uint16_t MIN_BYTE_SIZE = 11u;

        /**
         * \brief Creates a new ObjectDefinition instance.
         */
        ObjectDefinition();

        /**
         * \brief imports the provided data into the ObjectDefinition instance
         * \param data pointer to raw data array
         * \param size size of raw data array
         * \return number of bytes read from the data array
         */
        uint16_t import(const char *data, const uint16_t &size) override;

        // =======
        // Getters
        // =======

        /**
         * \brief Retrieves the ID of this ObjectDefinition instance
         * \return object ID
         */
        [[maybe_unused]] [[nodiscard]] const uint16_t &getId() const noexcept;

        /**
         * \brief Retrieves the version of this ObjectDefinition instance
         * \return object version
         */
        [[maybe_unused]] [[nodiscard]] const uint8_t &getVersion() const noexcept;

        /**
         * \brief Retrieves the sequenceFlag of this ObjectDefinition instance
         * \return sequence flag
         */
        [[maybe_unused]] [[nodiscard]] const SequenceFlag &getSequenceFlag() const noexcept;

        /**
         * \brief Retrieves the size of the encoded data contained in this ObjectDefinition instance
         * \return data length
         */
        [[maybe_unused]] [[nodiscard]] const uint32_t &getDataLength() const noexcept;

        /**
         * \brief Retrieves the width of the decompressed image made from the data in this ObjectDefinition instance
         * \return decompressed image width
         */
        [[maybe_unused]] [[nodiscard]] const uint16_t &getWidth() const noexcept;

        /**
         * \brief Retrieves the height of the decompressed image made from the data in this ObjectDefinition instance
         * \return decompressed image height
         */
        [[maybe_unused]] [[nodiscard]] const uint16_t &getHeight() const noexcept;

        /**
         * \brief Retrieves the compressed image data in this ObjectDefinition instance
         * \return compressed image data
         */
        [[maybe_unused]] [[nodiscard]] const std::vector<uint8_t> &getEncodedObjectData() const noexcept;

        /**
         * \brief Retrieves the decompressed image data in this ObjectDefinition instance
         * \return decompressed image data
         */
        [[maybe_unused]] [[nodiscard]] std::vector<std::vector<uint8_t>> getDecodedObjectData() const noexcept;
    };
}
