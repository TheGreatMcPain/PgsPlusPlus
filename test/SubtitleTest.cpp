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
#include <omp.h>

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

        this->fullSUPStream = std::ifstream("./res/subs.sup", std::ios::binary | std::ios::in | std::ios::ate);
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

TEST_F(SubtitleTest, exportShortSubtitleImages)
{
    const auto data = std::unique_ptr<char>(new char[this->shortFileSize]);
    this->shortSUPStream.readsome(data.get(), this->shortFileSize);

    vector<shared_ptr<Pgs::Subtitle>> subtitles;
    ASSERT_NO_THROW(subtitles = Pgs::Subtitle::createAll(data.get(), this->shortFileSize));

    fs::path outDir = fs::path("./out");
    fs::create_directories(outDir);

    const auto quantumScale = static_cast<uint32_t>((1 << QuantumDepth) - 1);

#pragma omp parallel for default(none) shared(quantumScale, subtitles) private(outDir)
    for (uint32_t i = 0; i < subtitles.size(); ++i)
    {
        const auto &sub = subtitles[i];

        outDir = fs::path("./out");
        if (!sub->containsImage())
        {
            continue;
        }

        const auto filePath = outDir.append(std::to_string(sub->getPresentationTimeMs()) + ".png");
        const auto imgData = sub->getImage(Pgs::ColorSpace::RGBA);
        const auto objHeight = sub->getOds(0)->getHeight();
        const auto objWidth = sub->getOds(0)->getWidth();

        Magick::Image image(Magick::Geometry(objWidth, objHeight),
                            Magick::Color(0, 0, UINT8_MAX, 0));
        image.modifyImage();
        image.type(Magick::ImageType::TrueColorMatteType);
        image.colorSpace(Magick::ColorspaceType::RGBColorspace);

        Magick::Pixels pixelView(image);
        for (uint16_t row = 0; row < objHeight; ++row)
        {
            for (uint16_t col = 0; col < objWidth; ++col)
            {
                const auto &color = imgData[row][col];
                const auto red = static_cast<uint16_t>(color[0]);
                const auto green = static_cast<uint16_t>(color[1]);
                const auto blue = static_cast<uint16_t>(color[2]);
                const auto alpha = static_cast<uint16_t>(color[3]);

                const auto scaledRed = static_cast<uint16_t>(
                        (static_cast<double>(red) / static_cast<double>(UINT8_MAX)) *
                        quantumScale);
                const auto scaledGreen = static_cast<uint16_t>(
                        (static_cast<double>(green) / static_cast<double>(UINT8_MAX)) *
                        quantumScale);
                const auto scaledBlue = static_cast<uint16_t>(
                        (static_cast<double>(blue) / static_cast<double>(UINT8_MAX)) *
                        quantumScale);
                const auto scaledAlpha = static_cast<uint16_t>(
                        (static_cast<double>(UINT8_MAX - alpha) / static_cast<double>(UINT8_MAX)) *
                        quantumScale);

                *(pixelView.get(col, row, 1, 1)) = Magick::Color(scaledRed, scaledGreen, scaledBlue, scaledAlpha);
            }
        }
        image.syncPixels();

        image.matte(true);
        image.magick("PNG32");

        image.write(filePath.string());
    }
}

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

