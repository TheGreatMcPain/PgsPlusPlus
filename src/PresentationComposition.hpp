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

#include <cstdint>
#include <vector>
#include <memory>

using std::shared_ptr;
using std::vector;

namespace Pgs
{
    /**
     * \brief Enumeration of possible composition states.
     */
    enum class CompositionState
    {
        Normal = 0x00, /**< Defines a <em>display update</em>. Contains only functional segments that differ from the preceding composition. Commonly used to stop the display of the previous display set. */
        AcquisitionPoint = 0x40, /**< Defines a <em>display refresh</em>. Segment contains data used to update the object with the same ID. */
        EpochStart = 0x80 /**< Defines a <em>new display</em>. Contains all functional segments needed to show a new composition. */
    };

    /**
     * \brief Individual Composition Object used to define display attributes of the subtitle image.
     */
    class CompositionObject
    {
    protected:
        uint16_t objectID; /**< ID of the associated Object Definition Segment. */
        uint8_t windowID; /**< ID of the associated Window Definition Segment. Up to 2 images can use 1 window. */
        bool croppedFlag; /**< Set to true to force display of the cropped image object; false, otherwise. */
        uint16_t hPos; /**< Horizontal (x) offset from the top-left pixel of the video frame. */
        uint16_t vPos; /**< Vertical (y) offset from the top-left pixel of the video frame. */
        uint16_t cropHPos; /**< Horizontal (x) crop offset from the top-left pixel of the video frame. */
        uint16_t cropVPos; /**< Vertical (y) crop offset from the top-left pixel of the video frame. */
        uint16_t cropWidth; /**< Width of the crop. */
        uint16_t cropHeight; /**< Height of the crop. */
    public:
        /**
         * \brief Minimum number of bytes needed to create a basic CompositionObject instance from
         * provided data.
         */
        static constexpr uint16_t MIN_DATA_SIZE = 8u;

        /**
         *  \brief Constructs a new CompositionObject instance.
         *
         *  \details
         *  This constructor should not be used by the end user. Use the create function instead.
         */
        CompositionObject();

        /**
         *  \brief Imports the provided data and creates a shared pointer to a new CompositionObject instance.
         *  \param data pointer to raw data array
         *  \param size size of the raw data array
         *  \param readPos point in array to read from.
         *
         *  \throws ImportException
         */
        static shared_ptr<CompositionObject> create(const char *data, const uint16_t &size, uint16_t &readPos);

        // =======
        // Getters
        // =======

        const uint16_t &getObjectID() const;

        const uint8_t &getWindowID() const;

        const bool &getCroppedFlag() const;

        const uint16_t &getHPos() const;

        const uint16_t &getVPos() const;

        const uint16_t &getCropHPos() const;

        const uint16_t &getCropVPos() const;

        const uint16_t &getCropWidth() const;

        const uint16_t &getCropHeight() const;
    };

    /**
     * \brief Presentation Composition structure containing all relevant display info for the subtitle image.
     */
    class PresentationComposition : public SegmentData
    {
    protected:
        uint16_t width;     /**< Pixels in video width. */
        uint16_t height;    /**< Pixels in video height */
        uint8_t frameRate; /**< Subtitle display frame rate. Should almost always be 0x10 (24fps) */
        uint16_t compositionNumber; /**< Index of this composition. This value is incremented by one for each graphics update. */
        CompositionState compositionState; /**< Type of composition state being defined. */
        bool paletteUpdateFlag; /**< True if this segment describes a <em>palette only</em> update; false, otherwise. */
        uint8_t paletteID; /**< ID of palette to use in palette-only update. */
        uint8_t compositionObjectCount; /**< Number of composition objects defined in segment. */
        vector<shared_ptr<CompositionObject>> compositionObjects; /**< Vector of Composition Objects in this segment. */
    public:
        /**
         * \brief Minimum number of bytes needed to create a basic PresentationComposition instance from
         * provided data.
         */
        static constexpr uint16_t MIN_DATA_SIZE = 11u;

        /**
         *  \brief Constructs a new PresentationComposition instance.
         * 
         *  \details
         *  To add data to the instance, use one of the import() methods.
         */
        PresentationComposition();

        uint16_t import(const char *data, const uint16_t &size) override;

        // =======
        // Getters
        // =======

        const uint16_t &getWidth() const;

        const uint16_t &getHeight() const;

        const uint8_t &getFrameRate() const;

        const uint16_t &getCompositionNumber() const;

        const CompositionState &getCompositionState() const;

        const bool &getPaletteUpdateFlag() const;

        const uint8_t &getPaletteID() const;

        const uint8_t &getCompositionObjectCount() const;

        const vector<shared_ptr<CompositionObject>> &getCompositionObjects() const;
    };
}
