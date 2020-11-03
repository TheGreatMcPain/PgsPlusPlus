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

uint16_t PaletteDefinition::import(const char *data, const uint16_t &size)
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

    return readPos;
}

uint16_t PaletteDefinition::import(const vector<char> &data)
{
    return this->import(data.data(), data.size());
}
