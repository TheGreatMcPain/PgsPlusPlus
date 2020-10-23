//
// Created by bdavidson on 10/22/20.
//

#include <gtest/gtest.h>

#include <fstream>
#include <pgs.hpp>
#include <Segment.hpp>

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

TEST_F(PgsTest, readPresentationCompositionSegement)
{
    const uint32_t dataSize = 32;
    this->supFileStream.seekg(std::ios::beg + 140);

    char *data = new char[dataSize];
    this->supFileStream.readsome(data, dataSize);

    Pgs::Segment segment;
    ASSERT_NO_THROW(segment.import(data, dataSize));
    delete[] data;
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
