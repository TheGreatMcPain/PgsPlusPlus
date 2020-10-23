#pragma once

#include <vector>

using std::vector;

namespace Pgs
{
    /**
     * \brief Abstract base class for classes used for storing segment data.
     */
    class SegmentData
    {
    public:
        SegmentData() = default;

        virtual ~SegmentData() noexcept = default;

        virtual void import(const char *data, const uint16_t &size) = 0;

        virtual void import(const vector<char> &data) = 0;
    };
}