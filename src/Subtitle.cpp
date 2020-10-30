//
// Created by bdavidson on 10/29/20.
//

#include "Subtitle.hpp"

using namespace Pgs;

Subtitle::Subtitle()
{
    this->presentationComposition = nullptr;
    this->windowDefinition = nullptr;
    this->paletteDefinition = nullptr;
    this->numObjectDefinitions = 0u;
    this->objectDefinitions[0] = nullptr;
    this->objectDefinitions[1] = nullptr;

    this->decodingTime = 0u;
    this->presentationTime = 0u;
}

void Subtitle::import(const Segment &segment)
{
    switch(segment.segmentType)
    {
        case SegmentType::PaletteDefinition:
            this->importPds(segment);
            break;
        case SegmentType::ObjectDefinition:
            this->importOds(segment);
            break;
        case SegmentType::PresentationComposition:
            this->importPcs(segment);
            break;
        case SegmentType::WindowDefinition:
            this->importWds(segment);
            break;
        case SegmentType::EndOfDisplaySet:
            this->importEnd(segment);
            break;
    }
}

void Subtitle::importPcs(const Segment &segment)
{

}

void Subtitle::importWds(const Segment &segment)
{

}

void Subtitle::importPds(const Segment &segment)
{

}

void Subtitle::importOds(const Segment &segment)
{

}

void Subtitle::importEnd(const Segment &segment)
{
    this->decodingTime = segment.decodingTimestamp / 90;
    this->presentationTime = segment.presentationTimestamp / 90;
}
