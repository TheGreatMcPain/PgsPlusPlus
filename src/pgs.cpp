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

#include <endian.h>

uint32_t Pgs::read4Bytes(const uint8_t *data, uint16_t &readPos)
{
    auto result = static_cast<uint32_t>((data[readPos] << 24u) | (data[readPos + 1] << 16u) |
                                        (data[readPos + 2] << 8u) |
                                        data[readPos + 3]);
    readPos += 4;
    return le32toh(result);
}

uint16_t Pgs::read2Bytes(const uint8_t *data, uint16_t &readPos)
{
    auto result = static_cast<uint16_t>((data[readPos] << 8u) | data[readPos+1]);
    readPos += 2;
    return le16toh(result);
}
