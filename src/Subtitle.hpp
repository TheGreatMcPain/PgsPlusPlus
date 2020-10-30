//
// Created by bdavidson on 10/29/20.
//

#pragma once

#include "Segment.hpp"

#include <cstdint>
#include <array>
#include <memory>

using std::array;
using std::shared_ptr;

namespace Pgs
{
    /**
     * \brief The Subtitle class takes the data from imported PGS segments, copies relevant data to its own instance,
     * and retains pointers to the imported segments so that a user may access less commonly-used data.
     *
     * \details
     * On segment import, this class <strong>copies</strong> relevant data to itself. While a pointer to the original
     * segment class is retained, updates to data stored in this class are not reflected in the associated segment
     * instance and vice versa. If the user wishes to update data in both places, they should first update the segment
     * instance then re-import it in the Subtitle instance.
     */
    class Subtitle
    {
    protected:
        /**
         * \brief Shared pointer to the imported presentation composition segment.
         */
        shared_ptr<Segment> presentationComposition;

        /**
         * \brief Shared pointer to the imported window definition segment.
         */
        shared_ptr<Segment> windowDefinition;

        /**
         * \brief Shared pointer to the imported palette definition segment.
         */
        shared_ptr<Segment> paletteDefinition;

        /**
         * \brief Number of object definition segments in this subtitle. Value can be 0-2.
         */
        uint8_t numObjectDefinitions;

        /**
         * \brief Array containing shared pointers to imported object definition segments.
         */
        array<shared_ptr<Segment>, 2> objectDefinitions;

        /**
         * \brief The time in milliseconds since the start of the stream when subtitle decoding should be finished.
         *
         * \details
         * Most applications will not need this value. It is present in PGS data to help extremely low-power devices
         * prioritize which subtitles to decode and when.
         * <br/><br/>As with presentationTime, the original 90kHz value can be retrieved from any of the stored Segment
         * object pointers.
         */
         uint32_t decodingTime;

        /**
         * \brief The time in milliseconds since the start of the stream when to present the subtitle.
         *
         * \details
         * The original timing data is sampled at 90kHz for ultra high-accuracy timing. This is significantly
         * more accurate than most application need, so the value here is rounded to the nearest millisecond.
         * If an application does need full accuracy of the timestamp, it can be retrieved through any of the
         * stored Segment object pointers.
         */
        uint32_t presentationTime;



        /**
         * \brief Imports a Segment containing presentation composition data.
         * \param segment Segment to import
         */
        void importPcs(const Segment &segment);

        /**
         * \brief Imports a Segment containing window definition data.
         * \param segment Segment to import
         */
        void importWds(const Segment &segment);

        /**
         * \brief Imports a Segment containing palette definition data.
         * \param segment Segment to import
         */
        void importPds(const Segment &segment);

        /**
         * \brief Imports a Segment containing object definition data.
         * \param segment Segment to import
         */
        void importOds(const Segment &segment);

        /**
         * \brief Imports an End Segment.
         *
         * \details
         * The only useful data in an End Segment is the display timing.
         *
         * \param segment Segment to import
         */
        void importEnd(const Segment &segment);
    public:
        /**
         * \brief Creates a new instance of Subtitle.
         */
        Subtitle();

        /**
         * \brief Imports any provided Segment into the Subtitle instance.
         *
         * \details
         * The actual import process is delegated to specific 'protected' methods for each Segment type.
         *
         * \param segment Segment to import
         */
        void import(const Segment &segment);
    };
}
