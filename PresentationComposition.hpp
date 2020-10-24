/*
 * Copyright (c) 2020 Brenden Davidson
 */

#pragma once

#include "SegmentData.hpp"

#include <cstdint>
#include <vector>

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
    public:
        uint16_t objectID; /**< ID of the associated Object Definition Segment. */
        uint8_t windowID; /**< ID of the associated Window Definition Segment. Up to 2 images can use 1 window. */
        bool croppedFlag; /**< Set to true to force display of the cropped image object; false, otherwise. */
        uint16_t hPos; /**< Horizontal (x) offset from the top-left pixel of the video frame. */
        uint16_t vPos; /**< Vertical (y) offset from the top-left pixel of the video frame. */
        uint16_t cropHPos; /**< Horizontal (x) crop offset from the top-left pixel of the video frame. */
        uint16_t cropVPos; /**< Vertical (y) crop offset from the top-left pixel of the video frame. */
        uint16_t cropWidth; /**< Width of the crop. */
        uint16_t cropHeight; /**< Height of the crop. */

        /**
         * \brief Minimum number of bytes needed to create a basic CompositionObject instance from
         * provided data.
         */
        static constexpr uint16_t MIN_DATA_SIZE = 8u;

        /**
         *  \brief Constructs a new CompositionObject instance.
         * 
         *  \details
         *  To add data to the instance, use one of the import() methods.
         */
        CompositionObject();

        /**
         *  \brief Imports the provided data into the CompositionObject instance.
         *  \param data pointer to raw data array
         *  \param size size of the raw data array
         *
         *  \throws ImportException
         */
        void import(const char *data, const uint16_t &size, uint16_t &readPos);
    };

    /**
     * \brief Presentation Composition structure containing all relevant display info for the subtitle image.
     */
    class PresentationComposition : public SegmentData
    {
    public:
        uint16_t width;     /**< Pixels in video width. */
        uint16_t height;    /**< Pixels in video height */
        uint8_t frameRate; /**< Subtitle display frame rate. Should almost always be 0x10 (24fps) */
        uint16_t compositionNumber; /**< Index of this composition. This value is incremented by one for each graphics update. */
        CompositionState compositionState; /**< Type of composition state being defined. */
        bool paletteUpdateFlag; /**< True if this segment describes a <em>palette only</em> update; false, otherwise. */
        uint8_t paletteID; /**< ID of palette to use in palette-only update. */
        uint8_t compositionObjectCount; /**< Number of composition objects defined in segment. */
        vector<CompositionObject> compositionObjects; /**< Vector of Composition Objects in this segment. */

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

        void import(const char *data, const uint16_t &size) override;

        [[maybe_unused]] void import(const vector<char> &data) override;
    };
}
