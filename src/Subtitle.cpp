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


#include "Subtitle.hpp"

using namespace Pgs;

CreateError::CreateError(const char *msg) : std::runtime_error(msg)
{}

Subtitle::Subtitle()
{
    this->presentationComposition = std::shared_ptr<PresentationComposition>();
    this->windowDefinition = std::shared_ptr<WindowDefinition>();
    this->paletteDefinition = std::shared_ptr<PaletteDefinition>();
    this->numObjectDefinitions = 0u;
    this->objectDefinitions[0] = std::shared_ptr<ObjectDefinition>();
    this->objectDefinitions[1] = std::shared_ptr<ObjectDefinition>();

    this->decodingTime = 0u;
    this->presentationTime = 0u;
}

Subtitle::~Subtitle() = default;

shared_ptr<Subtitle> Subtitle::create(const char *data, const uint32_t &size, uint32_t &readPos)
{
    /*
     * Continuously read through the data until either an End Segment is imported or the end of the data is reached.
     *
     * Each imported segment will return the number of bytes it read from the array, so the read position can be updated
     * that way.
     */

    auto subtitle = std::make_shared<Subtitle>(Subtitle());
    bool endReached = false;
    uint32_t segmentEnd;
    char buff[2];
    while(readPos < size && !endReached)
    {
        // Locate start of segment
        buff[0] = data[readPos];
        buff[1] = data[readPos+1];
        // The buffer must be used to make sure the readPos doesn't get messed up.
        if (buff[0] != 'P' && buff[1] != 'G')
        {
            readPos += 1;
            continue;
        }

        // Locate end of segment.
        segmentEnd = readPos + Segment::MIN_BYTE_SIZE + Segment::getSegmentSize(data+readPos, size-readPos);
        if (segmentEnd > size)
        {
            throw CreateError("Subtitle::create: Segment size larger than remaining data.");
        }

        Segment segment;
        readPos += segment.import(data + readPos, segmentEnd - readPos);

        const auto segType = subtitle->import(segment);
        endReached = segType == SegmentType::EndOfDisplaySet;
    }

    if (!endReached)
    {
        throw CreateError("Subtitle::create: Failed to create complete Subtitle.");
    }

    return subtitle;
}

shared_ptr<Subtitle> Subtitle::create(const vector<char> &data, uint32_t &readPos)
{
    return Subtitle::create(data.data(), data.size(), readPos);
}

Pgs::SegmentType Subtitle::import(const Segment &segment)
{
    switch(segment.segmentType)
    {
        case SegmentType::PaletteDefinition:
            this->paletteDefinition = std::dynamic_pointer_cast<PaletteDefinition>(segment.data);
            return SegmentType::PaletteDefinition;
        case SegmentType::ObjectDefinition:
            this->importOds(segment.data);
            return SegmentType::ObjectDefinition;
        case SegmentType::PresentationComposition:
            this->presentationComposition = std::dynamic_pointer_cast<PresentationComposition>(segment.data);
            return SegmentType::PresentationComposition;
        case SegmentType::WindowDefinition:
            this->windowDefinition = std::dynamic_pointer_cast<WindowDefinition>(segment.data);
            return SegmentType::WindowDefinition;
        case SegmentType::EndOfDisplaySet:
            this->importEnd(segment);
            return SegmentType::EndOfDisplaySet;
    }
    return SegmentType::EndOfDisplaySet;
}

void Subtitle::importOds(const shared_ptr<SegmentData> &segmentData)
{
    const auto ods = std::dynamic_pointer_cast<ObjectDefinition>(segmentData);
    switch (ods->sequenceFlag)
    {
        case SequenceFlag::Last:
            this->numObjectDefinitions = 2u;
            this->objectDefinitions[1] = ods;
            break;
        case SequenceFlag::First:
            this->numObjectDefinitions = 2u;
            this->objectDefinitions[0] = ods;
            break;
        case SequenceFlag::Only:
            this->numObjectDefinitions = 1u;
            this->objectDefinitions[0] = ods;
            break;
        default:
            this->numObjectDefinitions = 0u;
    }
}

void Subtitle::importEnd(const Segment &segment)
{
    this->decodingTime = segment.decodingTimestamp;
    this->presentationTime = segment.presentationTimestamp;
}

// =======
// Getters
// =======

shared_ptr<PresentationComposition> Subtitle::getPcs() const noexcept
{
    return this->presentationComposition;
}

shared_ptr<WindowDefinition> Subtitle::getWds() const noexcept
{
    return this->windowDefinition;
}

shared_ptr<PaletteDefinition> Subtitle::getPds() const noexcept
{
    return this->paletteDefinition;
}

shared_ptr<ObjectDefinition> Subtitle::getOds(const uint8_t &index) const noexcept
{
    if (index > 1)
    {
        return nullptr;
    }

    return this->objectDefinitions[index];
}

const uint32_t &Subtitle::getPresentationTime() const noexcept
{
    return this->presentationTime;
}

uint32_t Subtitle::getPresentationTimeMs() const noexcept
{
    return this->presentationTime / 90;
}

const uint32_t &Subtitle::getDecodingTime() const noexcept
{
    return this->decodingTime;
}

uint32_t Subtitle::getDecodingTimeMs() const noexcept
{
    return this->decodingTime / 90;
}


