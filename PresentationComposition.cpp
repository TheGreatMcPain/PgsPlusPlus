//
// Created by bdavidson on 10/23/20.
//

#include "PresentationComposition.hpp"

using namespace Pgs;

inline uint32_t read4Bytes(const uint8_t *data, uint16_t &readPos)
{
    auto result = static_cast<uint32_t>((data[readPos] << 24u) | (data[readPos + 1] << 16u) |
                                        (data[readPos + 2] << 8u) |
                                        data[readPos + 3]);
    readPos += 4;
    return result;
}

inline uint16_t read2Bytes(const uint8_t *data, uint16_t &readPos)
{
    auto result = static_cast<uint16_t>((data[readPos] << 8u) | data[readPos+1]);
    readPos += 2;
    return result;
}

CompositionObject::CompositionObject()
{
    this->objectID = 0;
    this->windowID = 0;
    this->croppedFlag = false;
    this->hPos = 0;
    this->vPos = 0u;
    this->cropHPos = 0u;
    this->cropVPos = 0u;
    this->cropWidth = 0u;
    this->cropHeight = 0;
}

CompositionObject::CompositionObject(const char *data, const uint16_t &size)
{
    const auto *byteData = reinterpret_cast<const uint8_t *>(data);
    uint16_t readPos = 0u;
    this->objectID = read2Bytes(byteData, readPos);
    this->windowID = byteData[readPos];
    ++readPos;
    this->croppedFlag = (byteData[readPos] == 0x40);
    ++readPos;
    this->hPos = read2Bytes(byteData, readPos);
    this->vPos = read2Bytes(byteData, readPos);

    if (this->croppedFlag)
    {
        this->cropHPos = read2Bytes(byteData, readPos);
        this->cropVPos = read2Bytes(byteData, readPos);
        this->cropWidth = read2Bytes(byteData, readPos);
        this->cropHeight = read2Bytes(byteData, readPos);
    }
    else
    {
        this->cropHPos = 0u;
        this->cropVPos = 0u;
        this->cropWidth = 0u;
        this->cropHeight = 0u;
    }
}

CompositionObject::CompositionObject(const vector<char> &data) : CompositionObject(data.data(), data.size())
{
}
