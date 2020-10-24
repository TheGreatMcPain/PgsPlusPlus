//
// Created by bdavidson on 10/23/20.
//

#include "SegmentData.hpp"

using namespace Pgs;

Pgs::ImportException::ImportException(const char *msg) : std::runtime_error(msg)
{
}

