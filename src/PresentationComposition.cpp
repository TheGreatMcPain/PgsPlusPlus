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

#include "pgs.hpp"

#include "PresentationComposition.hpp"

using namespace Pgs;

// ==========================
// Composition Object Methods
// ==========================

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

void CompositionObject::import(const char *data, const uint16_t &size, uint16_t &readPos)
{
    if(size < CompositionObject::MIN_DATA_SIZE)
    {
        throw ImportException("CompositionObject: Not enough data to import basic structure.");
    }

    const auto *byteData = reinterpret_cast<const uint8_t *>(data);
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

// ================================
// Presentation Composition Methods
// ================================

PresentationComposition::PresentationComposition() : SegmentData()
{
    this->width = 0u;
    this->height = 0u;
    this->frameRate = 0u;
    this->compositionNumber = 0u;
    this->compositionState = CompositionState::EpochStart;
    this->paletteUpdateFlag = false;
    this->paletteID = 0u;
    this->compositionObjectCount = 0u;
    this->compositionObjects = vector<CompositionObject>();
}

void PresentationComposition::import(const char *data, const uint16_t &size)
{
    if (size < PresentationComposition::MIN_DATA_SIZE)
    {
        throw ImportException("PresentationComposition: Not enough data to import basic structure.");
    }

    const auto *byteData = reinterpret_cast<const uint8_t *>(data);

    this->compositionObjects.clear();

    uint16_t readPos = 0u;
    this->width = read2Bytes(byteData, readPos);
    this->height = read2Bytes(byteData, readPos);
    this->frameRate = byteData[readPos];
    ++readPos;
    this->compositionNumber = read2Bytes(byteData, readPos);
    this->compositionState = CompositionState(byteData[readPos]);
    ++readPos;
    this->paletteUpdateFlag = (byteData[readPos] == 0x80);
    ++readPos;
    this->paletteID = byteData[readPos];
    ++readPos;
    this->compositionObjectCount = byteData[readPos];
    ++readPos;

    uint16_t remainingSize = size - readPos;
    if(remainingSize < CompositionObject::MIN_DATA_SIZE * this->compositionObjectCount)
    {
        throw ImportException("PresentationComposition: Not enough data to import all CompositionObjects.");
    }

    this->compositionObjects.resize(this->compositionObjectCount);
    for (uint8_t i = 0u; i < this->compositionObjectCount; ++i)
    {
        this->compositionObjects[i].import(data, remainingSize, readPos);
        remainingSize = size - readPos;
    }
}

void PresentationComposition::import(const vector<char> &data)
{
    this->import(data.data(), data.size());
}
