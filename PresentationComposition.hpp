/*
 * Copyright (c) 2020 Brenden Davidson
 */

#pragma once

#include <cstdint>
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;
using std::shared_ptr;

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
        /**
         * \brief Minimum number of bytes needed to create a basic CompositionObject instance from
         * provided data.
         */
        static constexpr uint16_t MIN_PCS_OBJECT_SIZE = 8u;

        uint16_t objectID; /**< ID of the associated Object Definition Segment. */
        uint8_t windowID; /**< ID of the associated Window Definition Segment. Up to 2 images can use 1 window. */
        bool croppedFlag; /**< Set to 0x40 to force display of the cropped image object; 0x00, otherwise. */
        uint16_t hPos; /**< Horizontal (x) offset from the top-left pixel of the video frame. */
        uint16_t vPos; /**< Vertical (y) offset from the top-left pixel of the video frame. */
        uint16_t cropHPos; /**< Horizontal (x) crop offset from the top-left pixel of the video frame. */
        uint16_t cropVPos; /**< Vertical (y) crop offset from the top-left pixel of the video frame. */
        uint16_t cropWidth; /**< Width of the crop. */
        uint16_t cropHeight; /**< Height of the crop. */

    public:
        CompositionObject();

        /**
         * \brief Constructs a new CompositionObject instance using the provided data.
         * \param data pointer to raw data array
         * \param size size of the raw data array
         */
        CompositionObject(const char *data, const uint16_t &size);

        /**
         * \brief Constructs a new CompositionObject instance using the provided data.
         * \param data a vector containing the raw data
         */
        explicit CompositionObject(const vector<char> &data);
    };

    /**
     * \brief Presentation Composition structure containing all relevant display info for the subtitle image.
     */
    class PresentationComposition
    {
    protected:
        /**
         * \brief Minimum number of bytes needed to create a basic PresentationComposition instance from
         * provided data.
         */
        static constexpr uint16_t MIN_PCS_SIZE = 11u;

        uint16_t width;     /**< Pixels in video width. */
        uint16_t height;    /**< Pixels in video height */
        uint8_t frameRate; /**< Subtitle display frame rate. Should almost always be 0x10 (24fps) */
        uint16_t compositionNumber; /**< Index of this composition. This value is incremented by one for each graphics update. */
        CompositionState compositionState; /**< Type of composition state being defined. */
        uint8_t paletteUpdateFlag; /**< Set to 0x80 if this segment describes a <em>palette only</em> update; 0x00, otherwise. */
        uint8_t paletteID; /**< ID of palette to use in palette-only update. */
        uint8_t compositionObjectCount; /**< Number of composition objects defined in segment. */
        vector<CompositionObject> compositionObjects; /**< Vector of Composition Objects in this segment. */
    public:

    };
}
