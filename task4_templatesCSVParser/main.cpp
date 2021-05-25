#include <string>

#include "include/tuplePrint.h"
#include "include/csvParser.h"

#include <gtest/gtest.h>

TEST(ParserTest, ParsesSimpleCSV)
{
    std::istringstream stream("2,3,str,c,1\n"
                              "1,2.15,str,5,0\n");

    CSVParser<int, double, std::string, char, bool> parser(stream);

    size_t i = 0;

    for (std::tuple<int, double, std::string, char, bool> t : parser)
    {
        if (i == 0)
        {
            ASSERT_EQ(std::get<0>(t), 2);
            ASSERT_DOUBLE_EQ(std::get<1>(t), 3);
            ASSERT_EQ(std::get<2>(t), "str");
            ASSERT_EQ(std::get<3>(t), 'c');
            ASSERT_EQ(std::get<4>(t), true);
        }
        else if (i == 1)
        {
            ASSERT_EQ(std::get<0>(t), 1);
            ASSERT_DOUBLE_EQ(std::get<1>(t), 2.15);
            ASSERT_EQ(std::get<2>(t), "str");
            ASSERT_EQ(std::get<3>(t), '5');
            ASSERT_EQ(std::get<4>(t), false);
        }
        i++;
    }
}

TEST(ParserTest, ParsesEmptyStream)
{
    std::istringstream stream1("");

    CSVParser<> p1(stream1);

    ASSERT_NO_THROW(for (const auto& t : p1) {});

    std::istringstream stream2("");

    CSVParser<int, char> p2(stream2);

    ASSERT_NO_THROW(for (const auto& t : p2) {});
}

TEST(ParserTest, ParsesEscapedCSVValues)
{
    std::istringstream stream(R"("Free trip to A,B","5.89","  Special rate ""1.79""")");

    CSVParser<std::string, double, std::string> parser(stream);

    for (const auto& t : parser)
    {
        ASSERT_EQ(std::get<0>(t), R"(Free trip to A,B)");
        ASSERT_DOUBLE_EQ(std::get<1>(t), 5.89);
        ASSERT_EQ(std::get<2>(t), R"(  Special rate "1.79")");
    }
}

TEST(ParserTest, SkipsFirstLines)
{
    std::istringstream s("string on first line,1\nstring on second line,2");

    CSVParser<std::string, int> parser(s, 1);

    for (const auto& t : parser)
    {
        ASSERT_EQ(std::get<0>(t), "string on second line");
        ASSERT_EQ(std::get<1>(t), 2);
    }
}

TEST(ParserTest, AcceptsCustomDelimiters)
{
    // value delimiter - '|'
    // line delimiter - '$'
    std::istringstream s("1|2.15|string1$2|3|string2");

    CSVParser<int, double, std::string> parser(s, 0, '$', '|');

    size_t i = 0;
    for (const auto& t : parser)
    {
        if (i == 0)
        {
            ASSERT_EQ(std::get<0>(t), 1);
            ASSERT_DOUBLE_EQ(std::get<1>(t), 2.15);
            ASSERT_EQ(std::get<2>(t), "string1");
        }
        else if (i == 1)
        {

            ASSERT_EQ(std::get<0>(t), 2);
            ASSERT_DOUBLE_EQ(std::get<1>(t), 3);
            ASSERT_EQ(std::get<2>(t), "string2");
        }
        i++;
    }
}

TEST(ParserTest, AcceptsCustomEscapeSymbol)
{
    // escape symbol - '$'
    std::istringstream s(R"(string1, $string with , and $$escape symbol$$ $)");

    CSVParser<std::string, std::string> p(s, 0, '\n', ',', '$');

    for (const auto& t : p)
    {
        ASSERT_EQ(std::get<0>(t), "string1");
        ASSERT_EQ(std::get<1>(t), R"( string with , and $escape symbol$ )");
    }
}

TEST(ParserTest, ParsesEscapedCSVWithCustomDelimiters)
{
    // value delimiter - '|'
    // line delimiter - '$'
    // escape symbol '"'
    std::istringstream s(R"(1|"string with ""escape symbols"""|2.15$2|"string with |"|3)");

    CSVParser<int, std::string, double> p(s, 0, '$', '|');

    size_t i = 0;
    for (const auto& t : p)
    {
        if (i == 0)
        {
            ASSERT_EQ(std::get<0>(t), 1);
            ASSERT_EQ(std::get<1>(t), R"(string with "escape symbols")");
            ASSERT_DOUBLE_EQ(std::get<2>(t), 2.15);
        }
        else if (i == 1)
        {
            ASSERT_EQ(std::get<0>(t), 2);
            ASSERT_EQ(std::get<1>(t), R"(string with |)");
            ASSERT_DOUBLE_EQ(std::get<2>(t), 3);
        }
        i++;
    }
}

// fails to parse value of different type
TEST(ParserTest, FailsToParseValueOfDifferentType)
{
    std::istringstream stream("2,3,str,c,1\n"
                              "1,2.15,str,5,0");

    // second type is int, but there is floating point number in stream
    CSVParser<int, int, std::string, char, bool> parser(stream);

    ASSERT_ANY_THROW(for (const auto& t : parser) {});
}

// fails to parse csv with less values than specified
TEST(ParserTest, FailsToParseCSVWithLessValues)
{
    std::istringstream stream("2,3,str,c\n"
                              "1,2.15,str,5");

    // no bool in stream at the end
    CSVParser<int, int, std::string, char, bool> parser(stream);

    ASSERT_ANY_THROW(for (const auto& t : parser) {});
}

// fails to parse csv with more values than specified
TEST(ParserTest, FailsToParseCSVWithMoreValues)
{
    std::istringstream stream("2,3,str,c\n"
                              "1,2.15,str,5");

    // last field in stream is not specified
    CSVParser<int, int, std::string> parser(stream);

    ASSERT_ANY_THROW(for (const auto& t : parser) {});
}

int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
