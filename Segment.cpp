//
// Created by bdavidson on 10/23/20.
//

#include "Segment.hpp"

#include <cstring>

using namespace Pgs;

Segment::Segment()
{
    this->presentationTimestamp = 0u;
    this->decodingTimestamp = 0u;
    this->segmentType = SegmentType::EndOfDisplaySet;
    this->segmentSize = 0u;
    this->data = nullptr;
}

void Segment::import(const char *inData, const uint32_t &size)
{
    const auto byteData = reinterpret_cast<const uint8_t *>(inData);

    if (!byteData)
    {
        throw ImportException("Segment: no import data provided.");
    }

    if (size < Segment::MIN_BYTE_SIZE)
    {
        throw ImportException("Segment: Not enough data to fill basic Segment.");
    }

    uint16_t readPos = 0u;
    strncpy(this->magicNumber, reinterpret_cast<const char*>(byteData + readPos), 2);
    readPos += 2;
    this->presentationTimestamp = read4Bytes(byteData, readPos);
    this->decodingTimestamp = read4Bytes(byteData, readPos);
    this->segmentType = SegmentType(byteData[readPos]);
    ++readPos;
    this->segmentSize = read2Bytes(byteData, readPos);

    const uint16_t remainingSize = size - readPos;
    switch (this->segmentType)
    {
        case SegmentType::PaletteDefinition:
            break;
        case SegmentType::ObjectDefinition:
            break;
        case SegmentType::PresentationComposition:
            this->data.reset(new PresentationComposition());
            this->data->import(inData + readPos, remainingSize);
            break;
        case SegmentType::WindowDefinition:
            break;
        case SegmentType::EndOfDisplaySet:
            break;
        default:
            throw ImportException("Segment: Unexpected SegmentType encountered");
    }
}

void Segment::import(const vector<char> &inData)
{
    this->import(inData.data(), inData.size());
}
