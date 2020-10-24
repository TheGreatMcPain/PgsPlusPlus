//
// Created by bdavidson on 10/23/20.
//

#include "WindowDefinition.hpp"

#include <endian.h>

using namespace Pgs;

WindowObject::WindowObject()
{
    this->id = 0u;
    this->hPos = 0u;
    this->vPos = 0u;
    this->width = 0u;
    this->height = 0u;
}

void WindowObject::import(const char *data, const uint16_t &size, uint16_t &readPos)
{
    if (!data)
    {
        throw ImportException("WindowObject: did not receive input data.");
    }

    if (size < WindowObject::MIN_BYTE_SIZE)
    {
        throw ImportException("WindowObject: Not enough data to import basic structure.");
    }

    const auto byteData = reinterpret_cast<const uint8_t *>(data);
    this->id = byteData[readPos];
    ++readPos;
    this->hPos = be16toh(read2Bytes(byteData, readPos));
    this->vPos = be16toh(read2Bytes(byteData, readPos));
    this->width = be16toh(read2Bytes(byteData, readPos));
    this->height = be16toh(read2Bytes(byteData, readPos));
}

// =========================
// Window Definition Methods
// =========================

WindowDefinition::WindowDefinition()
{
    this->numWindows = 0u;
    this->windowObjects = vector<WindowObject>();
}

void WindowDefinition::import(const char *data, const uint16_t &size)
{
    if (!data)
    {
        throw ImportException("WindowDefinition: did not receive input data.");
    }

    if (size < WindowDefinition::MIN_BYTE_SIZE)
    {
        throw ImportException("WindowDefinition: Not enough data to import basic structure.");
    }

    const auto byteData = reinterpret_cast<const uint8_t *>(data);

    uint16_t readPos = 0u;
    this->numWindows = byteData[readPos];

    uint16_t remainingSize = size - readPos;
    if (remainingSize < this->numWindows * WindowObject::MIN_BYTE_SIZE)
    {
        throw ImportException("WindowDefinition: Not enough data to import all CompositionObjects.");
    }

    this->windowObjects.resize(this->numWindows);
    for (uint8_t i = 0; i < this->numWindows; ++i)
    {
        this->windowObjects[i].import(data, remainingSize, readPos);
        remainingSize = size - readPos;
    }
}

void WindowDefinition::import(const vector<char> &data)
{
    this->import(data.data(), data.size());
}


