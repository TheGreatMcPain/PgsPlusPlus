//
// Created by bdavidson on 10/23/20.
//

#include "PaletteDefinition.hpp"

using namespace Pgs;

void PaletteEntry::import(const char *data, const uint16_t &size, uint16_t &readPos)
{
    if(!data)
    {
        throw ImportException("PaletteEntry: no data provided to import method.");
    }

    if(size < PaletteEntry::MIN_BYTE_SIZE)
    {
        throw ImportException("PaletteEntry: Insufficient data provided to successfully complete import.");
    }

    const auto byteData = reinterpret_cast<const uint8_t *>(data);

    this->id = byteData[readPos];
    ++readPos;
    this->y = byteData[readPos];
    ++readPos;
    this->cr = byteData[readPos];
    ++readPos;
    this->cb = byteData[readPos];
    ++readPos;
    this->alpha = byteData[readPos];
    ++readPos;
}

PaletteDefinition::PaletteDefinition() : SegmentData()
{
    this->id = 0u;
    this->version = 0u;
    this->numEntries = 0u;
    this->entries = vector<PaletteEntry>();
}

void PaletteDefinition::import(const char *data, const uint16_t &size)
{
    if(!data)
    {
        throw ImportException("PaletteDefinition: no data provided to import method.");
    }

    if(size < PaletteDefinition::MIN_BYTE_SIZE)
    {
        throw ImportException("PaletteDefinition: Insufficient data provided to successfully complete import.");
    }

    const auto byteData = reinterpret_cast<const uint8_t *>(data);
    uint16_t readPos = 0u;
    this->id = byteData[readPos];
    ++readPos;
    this->version = byteData[readPos];
    ++readPos;

    uint16_t remainingSize = size - readPos;

    this->numEntries = remainingSize / PaletteEntry::MIN_BYTE_SIZE;

    this->entries.resize(this->numEntries);
    for (uint8_t i = 0; i < this->numEntries; ++i)
    {
        this->entries[i].import(data, remainingSize, readPos);
        remainingSize = size - readPos;
    }
}

void PaletteDefinition::import(const vector<char> &data)
{
    this->import(data.data(), data.size());
}
