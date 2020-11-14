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

#include <stdexcept>

namespace Pgs
{
    /**
     * \brief Abstract base class for classes used for storing segment data.
     */
    class SegmentData
    {
    public:
        /**
         * \brief Constructs a new instance of SegmentData
         */
        SegmentData() = default;

        virtual ~SegmentData() noexcept = default;

        /**
         * \brief Imports the provided data into this SegmentData object.
         * \param data pointer to raw data array
         * \param size number of bytes in data array
         *
         * \throws ImportException
         */
        virtual uint16_t import(const char *data, const uint16_t &size) = 0;
    };

    /**
     * \brief Custom exception class used to signal a failure in the import process of a SignalData-derived
     * class.
     *
     * \details
     * This exception is nearly always caused by missing or insufficient data provided to an import function.
     * Any import function in a class derived from SegmentData will check if it was passed enough data and if enough
     * data was provided.
     */
    class ImportException : virtual public std::runtime_error
    {
    public:
        explicit ImportException(const char *msg);

        ~ImportException() noexcept override = default;
    };
}