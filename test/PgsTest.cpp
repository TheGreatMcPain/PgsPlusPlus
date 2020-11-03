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
#include <src/pgs.hpp>
#include <src/Segment.hpp>

class PgsTest : public ::testing::Test
{
protected:
    std::ifstream supFileStream;
    uint32_t fileSize;

    void SetUp() override
    {
        this->supFileStream.seekg(std::ios::beg);
    }

    PgsTest()
    {
        this->supFileStream = std::ifstream("./res/subs_short.sup", std::ios::binary | std::ios::in | std::ios::ate);
        this->fileSize = supFileStream.tellg();
        this->supFileStream.seekg(std::ios::beg);
    }

    ~PgsTest() override
    {
        this->supFileStream.close();
    }
};

TEST_F(PgsTest, importNullData)
{
    Pgs::Segment segment;
    ASSERT_THROW(segment.import(nullptr, 0), Pgs::ImportException);
}

// =========
// PCS Tests
// =========

TEST_F(PgsTest, importValidPcsSegment)
{
    const uint32_t dataSize = 32;
    this->supFileStream.seekg(std::ios::beg + 0x8C);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    Pgs::Segment segment;
    uint32_t readSize = 0u;
    ASSERT_NO_THROW(readSize = segment.import(data, dataSize));
    delete[] data;

    ASSERT_EQ(readSize, dataSize);
    ASSERT_EQ(segment.segmentType, Pgs::SegmentType::PresentationComposition);
}

TEST_F(PgsTest, importShortPcsData)
{
    const uint32_t dataSize = 5;
    this->supFileStream.seekg(std::ios::beg + 0x8C);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    Pgs::Segment segment;
    ASSERT_THROW(segment.import(data, dataSize), Pgs::ImportException);
    delete[] data;
}

// =========
// WDS Tests
// =========

TEST_F(PgsTest, importValidWdsSegment)
{
    const uint32_t dataSize = 23;
    this->supFileStream.seekg(std::ios::beg + 0xAC);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    Pgs::Segment segment;
    uint32_t readSize = 0u;
    ASSERT_NO_THROW(readSize = segment.import(data, dataSize));
    delete[] data;

    ASSERT_EQ(readSize, dataSize);
    ASSERT_EQ(segment.segmentType, Pgs::SegmentType::WindowDefinition);
}

TEST_F(PgsTest, importShortWdsSegment)
{
    const uint32_t dataSize = 5;
    this->supFileStream.seekg(std::ios::beg + 0xAC);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    Pgs::Segment segment;
    ASSERT_THROW(segment.import(data, dataSize), Pgs::ImportException);
    delete[] data;
}

// =========
// PDS Tests
// =========

TEST_F(PgsTest, importValidPdsSegment)
{
    const uint32_t dataSize = 650;
    this->supFileStream.seekg(std::ios::beg + 0xC3);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    Pgs::Segment segment;
    uint32_t readSize = 0u;
    ASSERT_NO_THROW(readSize = segment.import(data, dataSize));
    delete[] data;

    ASSERT_EQ(readSize, dataSize);
    ASSERT_EQ(segment.segmentType, Pgs::SegmentType::PaletteDefinition);
}

TEST_F(PgsTest, importShortPdsSegment)
{
    const uint32_t dataSize = 5;
    this->supFileStream.seekg(std::ios::beg + 0xC3);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    Pgs::Segment segment;
    ASSERT_THROW(segment.import(data, dataSize), Pgs::ImportException);
    delete[] data;
}

// =========
// ODS Tests
// =========

TEST_F(PgsTest, importValidOdsSegment)
{
    const uint32_t dataSize = 3368;
    this->supFileStream.seekg(std::ios::beg + 0x034D);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    Pgs::Segment segment;
    uint32_t readSize = 0u;
    ASSERT_NO_THROW(readSize = segment.import(data, dataSize));
    delete[] data;

    ASSERT_EQ(readSize, dataSize);
    ASSERT_EQ(segment.segmentType, Pgs::SegmentType::ObjectDefinition);
}

TEST_F(PgsTest, importShortOdsSegment)
{
    const uint32_t dataSize = 5;
    this->supFileStream.seekg(std::ios::beg + 0x034D);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    Pgs::Segment segment;
    ASSERT_THROW(segment.import(data, dataSize), Pgs::ImportException);
    delete[] data;
}

// ========
// END Test
// ========

TEST_F(PgsTest, importValidEndSegment)
{
    const uint32_t dataSize = 13u;
    this->supFileStream.seekg(std::ios::beg + 0x1075);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    Pgs::Segment segment;
    uint32_t readSize = 0u;
    ASSERT_NO_THROW(readSize = segment.import(data, dataSize));
    delete[] data;

    ASSERT_EQ(readSize, dataSize);
    ASSERT_EQ(segment.segmentType, Pgs::SegmentType::EndOfDisplaySet);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
