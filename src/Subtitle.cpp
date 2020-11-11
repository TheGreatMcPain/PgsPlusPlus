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

#include <iostream>

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

vector<shared_ptr<Subtitle>> Subtitle::createAll(const char *data, const uint32_t &size)
{
    if (data == nullptr || size == 0)
    {
        throw CreateError("Subtitle::createSubtitles: no data provided.");
    }

    uint32_t readPos = 0u;
    auto subtitles = vector<shared_ptr<Subtitle>>();
    uint32_t subtitleSize, readSize;
    uint16_t itr = 0;
    while(readPos < size-1)
    {
        subtitleSize = Subtitle::getSubtitleSize(data+readPos, size - readPos);
        try
        {
            readSize = 0;
            subtitles.push_back(Subtitle::create(data+readPos, subtitleSize, readSize));
            readPos += readSize;
        }
        catch (const std::runtime_error& err)
        {
            std::cerr << err.what() << " " << std::to_string(itr) << "\n";
        }
        ++itr;
    }

    return subtitles;
}

Pgs::SegmentType Subtitle::import(const Segment &segment)
{
    switch(segment.getSegmentType())
    {
        case SegmentType::PaletteDefinition:
            this->paletteDefinition = std::dynamic_pointer_cast<PaletteDefinition>(segment.getData());
            return SegmentType::PaletteDefinition;
        case SegmentType::ObjectDefinition:
            this->importOds(segment.getData());
            return SegmentType::ObjectDefinition;
        case SegmentType::PresentationComposition:
            this->presentationComposition = std::dynamic_pointer_cast<PresentationComposition>(segment.getData());
            return SegmentType::PresentationComposition;
        case SegmentType::WindowDefinition:
            this->windowDefinition = std::dynamic_pointer_cast<WindowDefinition>(segment.getData());
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
    switch (ods->getSequenceFlag())
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
    this->decodingTime = segment.getDecodingTimestamp();
    this->presentationTime = segment.getPresentationTimestamp();
}

// =======
// Getters
// =======

uint32_t Subtitle::getSubtitleSize(const char *data, const uint32_t &size)
{
    const auto *byteData = reinterpret_cast<const uint8_t *>(data);

    uint32_t readPos = 0u;
    SegmentType segmentType = SegmentType::PresentationComposition;
    char buff[2];
    while (readPos < size && segmentType != SegmentType::EndOfDisplaySet)
    {
        // Find start of segment
        buff[0] = byteData[readPos];
        buff[1] = byteData[readPos+1];
        if (buff[0] != 'P' && buff[1] != 'G')
        {
            ++readPos;
            continue;
        }

        // Get segment type
        readPos += 10;
        segmentType = SegmentType(byteData[readPos]);
        ++readPos;

        // Get segment size and skip to end of segment.
        const uint16_t segmentSize = read2Bytes(reinterpret_cast<const uint8_t *>(byteData),
                                                reinterpret_cast<uint16_t &>(readPos));
        readPos += segmentSize;
        // Repeat until the end segment is found or out of data.
    }

    return readPos;
}

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


