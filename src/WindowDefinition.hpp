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

#include "pgs.hpp"
#include "SegmentData.hpp"

#include <cstdint>
#include <vector>

using std::vector;

namespace Pgs
{
    /**
     * \brief Individual window object definition
     *
     * \details
     * Contains data relevant to the scale and positioning of the subtitle image.
     */
    class WindowObject
    {
    public:
        /**
         * \brief Minimum number of bytes required to successfully import data.
         */
        static constexpr uint16_t MIN_BYTE_SIZE = 9u;

        uint8_t id; /**< Window ID */
        uint16_t hPos; /**< Horizontal (x) offset from the top-left pixel of the video frame. */
        uint16_t vPos; /**< Vertical (y) offset from the top-left pixel of the video frame. */
        uint16_t width; /**< Window width */
        uint16_t height; /**< Window height */

        /**
         * \brief Creates a new WindowObject instance.
         */
        WindowObject();

        /**
         * \brief imports the provided data into the class instance
         *
         * \details
         * The import process should always be handled by the WindowDefinition instance in
         * control of this WindowObject instance. It is not necessary for the user to directly
         * use this method.
         *
         * \param data pointer to raw import data array
         * \param size number of bytes in provided data array
         * \param readPos position in data array to begin reading from
         *
         * \throws ImportException
         */
        void import(const char* data, const uint16_t &size, uint16_t &readPos);
    };

    /**
     * \brief Contains the Window Definitions for a subtitle.
     */
    class WindowDefinition : public SegmentData
    {
    public:
        /**
         * \brief Minimum number of bytes required to successfully import data.
         */
        static constexpr uint16_t MIN_BYTE_SIZE = 1u;

        uint8_t numWindows; /**< Number of Windows contained in this segment. */
        vector<WindowObject> windowObjects; /**< Vector of WindowObjects managed by this class instance */

        /**
         * \brief Creates a new WindowDefinition instance
         */
        WindowDefinition();

        /**
         * \brief imports the provided data into the class instance
         * \param data pointer to raw data array to import
         * \param size number of bytes in provided data array
         */
        uint16_t import(const char *data, const uint16_t &size) override;
    };
}
