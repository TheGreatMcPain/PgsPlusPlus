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

#include <fstream>
#include <src/Subtitle.hpp>
#include <memory>
#include <vector>

using std::vector;
using std::shared_ptr;

class SubtitleTest : public ::testing::Test
{
protected:
    std::ifstream supFileStream;
    uint32_t fileSize;

    void SetUp() override
    {
        this->supFileStream.seekg(std::ios::beg);
    }

    SubtitleTest()
    {
        this->supFileStream = std::ifstream("./res/subs_short.sup", std::ios::binary | std::ios::in | std::ios::ate);
        this->fileSize = supFileStream.tellg();
        this->supFileStream.seekg(std::ios::beg);
    }

    ~SubtitleTest() override
    {
        this->supFileStream.close();
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
    this->supFileStream.seekg(std::ios::beg + 0x8C);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    shared_ptr<Pgs::Subtitle> subtitle;
    uint32_t readPos = 0u;
    ASSERT_NO_THROW(subtitle = Pgs::Subtitle::create(data, dataSize, readPos));
    delete[] data;

    ASSERT_EQ(readPos, dataSize);
}

TEST_F(SubtitleTest, importLargeSubtitle)
{
    const uint32_t dataSize = 27120;
    this->supFileStream.seekg(std::ios::beg + 0x17BFD6);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    shared_ptr<Pgs::Subtitle> subtitle;
    uint32_t readPos = 0u;
    ASSERT_NO_THROW(subtitle = Pgs::Subtitle::create(data, dataSize, readPos));
    delete[] data;

    ASSERT_EQ(readPos, dataSize);
}

TEST_F(SubtitleTest, importEmptySubtitle)
{
    const uint32_t dataSize = 60;
    this->supFileStream.seekg(std::ios::beg + 0x17BF9A);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    shared_ptr<Pgs::Subtitle> subtitle;
    uint32_t readPos = 0u;
    ASSERT_NO_THROW(subtitle = Pgs::Subtitle::create(data, dataSize, readPos));
    delete[] data;

    ASSERT_EQ(readPos, dataSize);
}

TEST_F(SubtitleTest, importAllSubtitles)
{
    char *data = new char[this->fileSize];
    this->supFileStream.readsome(data, this->fileSize);

    vector<shared_ptr<Pgs::Subtitle>> subtitles;
    subtitles = Pgs::Subtitle::createAll(data, this->fileSize);
    delete[] data;

}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

