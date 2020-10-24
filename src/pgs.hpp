#pragma once

#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <string>

namespace Pgs
{
    uint32_t read4Bytes(const uint8_t *data, uint16_t &readPos);
    uint16_t read2Bytes(const uint8_t *data, uint16_t &readPos);
}
