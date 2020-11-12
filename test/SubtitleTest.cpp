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

#include <gtest/gtest.h>
#include <GraphicsMagick/Magick++.h>

#include <src/Subtitle.hpp>
#include <fstream>
#include <memory>
#include <vector>
#include <filesystem>
#include <string>

using std::vector;
using std::shared_ptr;

namespace fs = std::filesystem;

class SubtitleTest : public ::testing::Test
{
protected:
    std::ifstream shortSUPStream;
    uint32_t shortFileSize;
    std::ifstream fullSUPStream;
    uint32_t fullFileSize;

    void SetUp() override
    {
        this->shortSUPStream.seekg(std::ios::beg);
        this->fullSUPStream.seekg(std::ios::beg);

        fs::remove_all("./out");
    }

    SubtitleTest()
    {
        this->shortSUPStream = std::ifstream("./res/subs_short.sup", std::ios::binary | std::ios::in | std::ios::ate);
        if (!this->shortSUPStream.is_open())
        {
            throw std::runtime_error("Failed to open short subtitle test file.");
        }
        this->shortFileSize = shortSUPStream.tellg();
        this->shortSUPStream.seekg(std::ios::beg);

        this->fullSUPStream = std::ifstream ("./res/subs.sup", std::ios::binary | std::ios::in | std::ios::ate);
        if (!this->fullSUPStream.is_open())
        {
            throw std::runtime_error("Failed to open large subtitle test file.");
        }
        this->fullFileSize = fullSUPStream.tellg();
        this->fullSUPStream.seekg(std::ios::beg);
    }

    ~SubtitleTest() override
    {
        this->shortSUPStream.close();
        this->fullSUPStream.close();
    }
};

TEST_F(SubtitleTest, importNullData)
{
    uint32_t readPos = 0u;
    ASSERT_THROW(Pgs::Subtitle::create(nullptr, 0, readPos), Pgs::CreateError);
}

TEST_F(SubtitleTest, importSmallSubtitle)
{
    const uint32_t dataSize = 4086;
    this->shortSUPStream.seekg(std::ios::beg + 0x8C);

    char *data = new char[dataSize];
    this->shortSUPStream.readsome(data, dataSize);

    shared_ptr<Pgs::Subtitle> subtitle;
    uint32_t readPos = 0u;
    ASSERT_NO_THROW(subtitle = Pgs::Subtitle::create(data, dataSize, readPos));
    delete[] data;

    ASSERT_EQ(readPos, dataSize);
}

TEST_F(SubtitleTest, importLargeSubtitle)
{
    const uint32_t dataSize = 27120;
    this->shortSUPStream.seekg(std::ios::beg + 0x17BFD6);

    char *data = new char[dataSize];
    this->shortSUPStream.readsome(data, dataSize);

    shared_ptr<Pgs::Subtitle> subtitle;
    uint32_t readPos = 0u;
    ASSERT_NO_THROW(subtitle = Pgs::Subtitle::create(data, dataSize, readPos));
    delete[] data;

    ASSERT_EQ(readPos, dataSize);
}

TEST_F(SubtitleTest, importEmptySubtitle)
{
    const uint32_t dataSize = 60;
    this->shortSUPStream.seekg(std::ios::beg + 0x17BF9A);

    char *data = new char[dataSize];
    this->shortSUPStream.readsome(data, dataSize);

    shared_ptr<Pgs::Subtitle> subtitle;
    uint32_t readPos = 0u;
    ASSERT_NO_THROW(subtitle = Pgs::Subtitle::create(data, dataSize, readPos));
    delete[] data;

    ASSERT_EQ(readPos, dataSize);
}

TEST_F(SubtitleTest, importShortSubtitleFile)
{
    const auto data = std::unique_ptr<char>(new char[this->shortFileSize]);
    this->shortSUPStream.readsome(data.get(), this->shortFileSize);

    vector<shared_ptr<Pgs::Subtitle>> subtitles;
    ASSERT_NO_THROW(subtitles = Pgs::Subtitle::createAll(data.get(), this->shortFileSize));
}

//TEST_F(SubtitleTest, exportShortSubtitleImages)
//{
//    const auto data = std::unique_ptr<char>(new char[this->shortFileSize]);
//    this->shortSUPStream.readsome(data.get(), this->shortFileSize);
//
//    vector<shared_ptr<Pgs::Subtitle>> subtitles;
//    ASSERT_NO_THROW(subtitles = Pgs::Subtitle::createAll(data.get(), this->shortFileSize));
//
//    fs::path outDir = fs::path("./out");
//    fs::create_directories(outDir);
//
//    for (const auto &sub : subtitles)
//    {
//        const std::string fileName = std::to_string(sub->getPresentationTimeMs()) + ".png";
//        const auto numODS = sub->getNumObjectDefinitions();
//
//        Magick::Image image = Magick::Image();
//
//        for (uint8_t i = 0; i < numODS; ++i)
//        {
//            const auto objectData = sub->getOds(i)->getDecodedObjectData();
//
//        }
//    }
//}

TEST_F(SubtitleTest, importFullSubtitleFile)
{
    const auto data = std::unique_ptr<char>(new char[this->fullFileSize]);
    this->fullSUPStream.readsome(data.get(), this->fullFileSize);

    vector<shared_ptr<Pgs::Subtitle>> subtitles;
    ASSERT_NO_THROW(subtitles = Pgs::Subtitle::createAll(data.get(), this->fullFileSize));
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    Magick::InitializeMagick(nullptr);
    return RUN_ALL_TESTS();
}

