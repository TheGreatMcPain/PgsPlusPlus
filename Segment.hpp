#pragma once

#include "pgs.hpp"
#include "PresentationComposition.hpp"

#include <vector>
#include <cstdint>
#include <memory>

using std::vector;
using std::unique_ptr;
using std::shared_ptr;

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
        static constexpr uint16_t MIN_BYTE_SIZE = 13u;

        char magicNumber[2]{};             /**< 2-byte value defining the start of a segment. Should always be PGS_MAGIC_NUMBER */
        uint32_t presentationTimestamp;  /**< 32-bit value defining the presentation time of the segment. */
        uint32_t decodingTimestamp;      /**< 32-bit value defining the decoding time of the segment. */
        SegmentType segmentType;         /**< Type of data contained in this segment. */
        uint16_t segmentSize;            /**< Number of bytes containing the segment data. */
        shared_ptr<SegmentData> data;    /**< Shared pointer to the corresponding data container. */
    public:
        Segment();

        void import(const char *inData, const uint32_t &size);

        void import(const vector<char> &inData);
    };
}