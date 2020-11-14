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
     * \brief Enumeration of data segment types found in Presentation Graphic Stream data.
     */
    enum class SegmentType
    {
        PaletteDefinition = 0x14,       /**< Segment contains palette information for the display set. */
        ObjectDefinition = 0x15,        /**< Segment contains compressed data defining all or part of a subtitle image. */
        PresentationComposition = 0x16, /**< Segment contains composition information for the display set. */
        WindowDefinition = 0x17,        /**< Segment contains information on the region of the display used for showing the subtitle. */
        EndOfDisplaySet = 0x80          /**< Segment denotes the end of a display set. No information is contained. */
    };

    /**
     * \brief Full object definition of a PGS data segment.
     *
     * \details
     * Each Segment contains some header data (everything from the magicNumber to the segmentSize) along with a shared
     * pointer to the corresponding data object.
     */
    class Segment
    {
    protected:
        char magicNumber[2]{};             /**< 2-byte value defining the start of a segment. Should always be PGS_MAGIC_NUMBER */
        uint32_t presentationTimestamp;  /**< 32-bit value defining the presentation time of the segment. */
        uint32_t decodingTimestamp;      /**< 32-bit value defining the decoding time of the segment. */
        SegmentType segmentType;         /**< Type of data contained in this segment. */
        uint16_t segmentSize;            /**< Number of bytes containing the segment data. */
        std::shared_ptr<SegmentData> data;    /**< Shared pointer to the corresponding data container. */
    public:
        static constexpr uint16_t MIN_BYTE_SIZE = 13u;

        Segment();

        ~Segment();

        /**
         * \brief Gets the size of a single segment in the provided data.
         * \param data raw data array to read from
         * \param size size of provided array
         * \return size of first detected segment.
         */
        static uint16_t getSegmentSize(const char *data, const uint16_t &size) noexcept;

        uint16_t import(const char *inData, const uint32_t &size);

        uint16_t import(const std::vector<char> &inData);

        // =======
        // Getters
        // =======

        /**
         * \brief Returns the magic number from the Segment.
         *
         * \details
         * This is unlikely to be useful unless the user needs troubleshoot data acquisition.
         *
         * \return 2 characters making up the Segment's magic number.
         */
        [[maybe_unused]] [[nodiscard]] const char *getMagicNumber() const;

        /**
         * \brief Gets the presentationTimestamp
         * \return presentationTimestamp
         */
        [[nodiscard]] const uint32_t &getPresentationTimestamp() const;

        /**
         * \brief Gets the decodingTimestamp
         * \return decodingTimestamp
         */
        [[nodiscard]] const uint32_t &getDecodingTimestamp() const;

        /**
         * \brief Gets the segmentType
         * \return segmentType
         */
        [[nodiscard]] const SegmentType &getSegmentType() const;

        /**
         * \brief Gets the segmentSize
         * \return segmentSize
         */
        [[maybe_unused]] [[nodiscard]] const uint16_t &getSegmentSize() const;

        /**
         * \brief Retrieves a shared pointer to the stored SegmentData object.
         * \return shared pointer to stored SegmentData.
         */
        [[nodiscard]] std::shared_ptr<SegmentData> getData() const;
    };
}