#pragma once

#include <vector>
#include <stdexcept>

using std::vector;

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
        virtual void import(const char *data, const uint16_t &size) = 0;

        /**
         * \brief Imports the provided data into this SegmentData object.
         * \param data vector containing the data to be imported
         *
         * \throws ImportException
         */
        virtual void import(const vector<char> &data) = 0;
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