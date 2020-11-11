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

#include "Segment.hpp"

#include <cstdint>
#include <array>
#include <vector>
#include <memory>
#include <stdexcept>

using std::vector;
using std::array;
using std::shared_ptr;

namespace Pgs
{
    class CreateError : virtual public std::runtime_error
    {
    public:
        explicit CreateError(const char *msg);

        ~CreateError() noexcept override = default;
    };

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
        shared_ptr<PresentationComposition> presentationComposition;

        /**
         * \brief Shared pointer to the imported window definition segment.
         */
        shared_ptr<WindowDefinition> windowDefinition;

        /**
         * \brief Shared pointer to the imported palette definition segment.
         */
        shared_ptr<PaletteDefinition> paletteDefinition;

        /**
         * \brief Number of object definition segments in this subtitle. Value can be 0-2.
         */
        uint8_t numObjectDefinitions;

        /**
         * \brief Array containing shared pointers to imported object definition segments.
         */
        array<shared_ptr<ObjectDefinition>, 2> objectDefinitions;

        /**
         * \brief The time since the start of the stream when subtitle decoding should be finished.
         *
         * \details
         * Most applications will not need this value. It is present in PGS data to help extremely low-power devices
         * prioritize which subtitles to decode and when.
         * <br/><br/>As with presentationTime, this value has an accuracy of 90kHz.
         */
         uint32_t decodingTime;

        /**
         * \brief The time since the start of the stream when to present the subtitle.
         *
         * \details
         * This value has an accuracy of 90kHz.
         */
        uint32_t presentationTime;

        /**
         * \brief Imports the provided SegmentData as an ObjectDefinition.
         *
         * \details
         * A single Subtitle can have up to 2 ObjectDefinitions, so this may be called multiple times.
         *
         * \param segmentData SegmentData to import
         */
        void importOds(const shared_ptr<SegmentData> &segmentData);

        /**
         * \brief Imports an End Segment.
         *
         * \details
         * The only useful data in an End Segment is the display timing.
         *
         * \param segment Segment to import
         */
        void importEnd(const Segment &segment);

        static uint32_t getSubtitleSize(const char *data, const uint32_t &size);
    public:
        /**
         * \brief Creates a new instance of Subtitle.
         */
        Subtitle();

        ~Subtitle();

        /**
         * \brief Generates a shared pointer to a new Subtitle instance and imports the provided data.
         * \param data pointer to raw data array
         * \param size size of the data array
         * \param readPos reference to position to start reading the data array from
         * \return shared pointer to new Subtitle instance.
         *
         * \throws CreateError
         */
        static shared_ptr<Subtitle> create(const char *data, const uint32_t &size, uint32_t &readPos);

        /**
         * \brief Generates a shared pointer to a new Subtitle instance and imports the provided data.
         * \param data raw data vector
         * \param readPos reference to position to start reading the data array from
         * \return shared pointer to new Subtitle instance.
         *
         * \throws CreateError
         */
        [[maybe_unused]] static shared_ptr<Subtitle> create(const std::vector<char> &data, uint32_t &readPos);

        /**
         * \brief Creates a vector of shared pointer to the Subtitle instance created from the provided data.
         * \param data pointer to raw data array.
         * \param size number of bytes in raw data array
         * \return vector of shared pointers to newly created Subtitle instances
         */
        static vector<shared_ptr<Subtitle>> createAll(const char *data, const uint32_t &size);

        /**
         * \brief Imports any provided Segment into the Subtitle instance.
         *
         * \details
         * The actual import process may be delegated to specific 'protected' methods for some Segment types.
         *
         * \param segment Segment to import
         */
        Pgs::SegmentType import(const Segment &segment);

        // =======
        // Getters
        // =======

        /**
         * \brief Retrieves a shared pointer to the PresentationComposition attached to this instance.
         * \return shared pointer to the PresentationComposition
         */
        [[maybe_unused]] shared_ptr<PresentationComposition> getPcs() const noexcept;

        /**
         * \brief Retrieves a shared pointer to the WindowDefinition attached to this instance.
         * \return shared pointer to the WindowDefinition
         */
        [[maybe_unused]] shared_ptr<WindowDefinition> getWds() const noexcept;

        /**
         * \brief Retrieves a shared pointer to the PaletteDefinition attached to this instance.
         * \return shared pointer to the PaletteDefinition
         */
        [[maybe_unused]] shared_ptr<PaletteDefinition> getPds() const noexcept;

        /**
         * \brief Retrieves a shared pointer to the selected ObjectDefinition attached to this instance.
         *
         * \details
         * There may only be up to 2 ObjectDefinitions associated with an instance. In the event that an index outside of
         * that range is provided, the method will return a null shared_ptr.
         *
         * \param index index of ObjectDefinition to access.
         * \return shared pointer to selected ObjectDefinition
         */
        [[maybe_unused]] shared_ptr<ObjectDefinition> getOds(const uint8_t &index) const noexcept;

        /**
         * \brief Retrieves the presentationTime exactly as it's stored in this instance.
         * \return presentationTime with 90kHz accuracy
         */
        [[maybe_unused]] const uint32_t &getPresentationTime() const noexcept;

        /**
         * \brief Retrieves the presentationTime as a millisecond value.
         * \return presentationTime with millisecond accuracy
         */
        [[maybe_unused]] uint32_t getPresentationTimeMs() const noexcept;

        /**
         * \brief Retrieves the decodingTime exactly as it's stored in this instance.
         * \return decodingTime with 90kHz accuracy
         */
        [[maybe_unused]] const uint32_t &getDecodingTime() const noexcept;

        /**
         * \brief Retrieves the decodingTime as a millisecond value.
         * \return decodingTime with millisecond accuracy
         */
        [[maybe_unused]] uint32_t getDecodingTimeMs() const noexcept;
    };
}
