#pragma once

#include <string>
#include <exception>
#include <sstream>
#include <istream>
#include <iterator>
#include <memory>
#include <cxxabi.h>
#include <vector>

template<typename T>
std::string GetTypeName()
{
    int status;
    std::string tname = typeid(T).name();
    char* demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    if (status == 0)
    {
        tname = demangled_name;
        std::free(demangled_name);
    }
    return tname;
}

class CSVParsingException : public std::exception
{
public:
    CSVParsingException(const std::string& msg, const std::string& csvLine, size_t errorPos, size_t lineNumber);

    const char* what() const noexcept override;

    virtual ~CSVParsingException() noexcept {}

protected:
    std::unique_ptr<std::string> whatMsg;
};

template<typename ... Args>
struct CSVReader
{
public:
    explicit CSVReader(char lineDelimiter = '\n', char valueDelimiter = ',', char escapeChar = '\"') :
            lineDelim(lineDelimiter),
            valueDelim(valueDelimiter),
            escapeChar(escapeChar) {}

    void ReadIntoTuple(std::istream& i, std::tuple<Args...>& tuple)
    {
        ReadLine(i, tuple, std::make_index_sequence<sizeof...(Args)>());
    }

    static bool CanRead(std::istream& i)
    {
        return i.good() && i.peek() != EOF;
    }

private:
    std::string row;
    const char lineDelim;
    const char valueDelim;
    const char escapeChar;
    size_t linesRead = 0;
    size_t curValuePos = 0;
    std::vector<std::string> rowFields;
    std::vector<size_t> delimitersPos;
    size_t curFieldIndex = 0;

    enum class CSVState
    {
        UnescapedField,
        EscapedField,
        EscapedEscape,
    };

    void SplitIntoStrings()
    {
        CSVState state = CSVState::UnescapedField;
        rowFields.clear();
        rowFields.push_back("");
        delimitersPos.clear();
        size_t i = 0; // index of the current field
        size_t curPos = 0;
        for (char c : row)
        {
            switch (state)
            {
                case CSVState::UnescapedField:
                    if (c == valueDelim)
                    {
                        rowFields.push_back("");
                        delimitersPos.push_back(curPos);
                        i++;
                    }
                    else if (c == escapeChar)
                    {
                        state = CSVState::EscapedField;
                    }
                    else
                    {
                        rowFields[i].push_back(c);
                    }
                    break;
                case CSVState::EscapedField:
                    if (c == escapeChar)
                    {
                        state = CSVState::EscapedEscape;
                    }
                    else
                    {
                        rowFields[i].push_back(c);
                    }
                    break;
                case CSVState::EscapedEscape:
                    if (c == valueDelim)
                    {
                        rowFields.push_back("");
                        delimitersPos.push_back(curPos);
                        i++;
                        state = CSVState::UnescapedField;
                    }
                    else if (c == escapeChar)
                    {
                        rowFields[i].push_back(escapeChar);
                        state = CSVState::EscapedField;
                    }
                    else
                    {
                        state = CSVState::UnescapedField;
                    }
                    break;
            }
            curPos++;
        }
    }

    template<size_t ... I>
    void ReadLine(std::istream& i, std::tuple<Args...>& tuple, std::index_sequence<I...>)
    {
        std::getline(i, row, lineDelim);
        linesRead++;
        curValuePos = 0;
        curFieldIndex = 0;

        if (row.empty())
        {
            throw CSVParsingException("Line was empty", row, curValuePos + 1, linesRead);
        }

        SplitIntoStrings();

        // C++17 fold expression
        (..., (ReadValue(std::get<I>(tuple))));

        if (curFieldIndex != rowFields.size())
        {
            std::stringstream ss;
            ss << "Unexpected value: " << rowFields[curFieldIndex];
            throw CSVParsingException(ss.str(), row, curValuePos + 1, linesRead);
        }
    }

    template<typename T>
    void ReadValue(T& t)
    {
        std::string curField = rowFields[curFieldIndex];
        std::stringstream convert(curField);
        if ((convert >> t).fail() || !(convert >> std::ws).eof())
        {
            std::stringstream ss;
            ss << "Couldn't parse value (expected type: " << GetTypeName<T>() << "): " << curField;
            throw CSVParsingException(ss.str(), row, curValuePos + 1, linesRead);
        }
        curValuePos = delimitersPos[curFieldIndex] + 1;
        curFieldIndex++;
    }

    void ReadValue(std::string& t)
    {
        t = rowFields[curFieldIndex];
        curValuePos = delimitersPos[curFieldIndex] + 1;
        curFieldIndex++;
    }
};

template<typename ... Args>
class CSVIterator
{
public:
    typedef std::input_iterator_tag iterator_tag;
    typedef std::tuple<Args...> value_type;
    typedef std::size_t difference_type;
    typedef std::tuple<Args...>* pointer;
    typedef std::tuple<Args...>& reference;

    explicit CSVIterator(std::istream& s, char lineDelimiter = '\n', char valueDelimiter = ',',
                         char escapeChar = '\"',
                         size_t alreadyReadLines = 0) :
            is(s),
            canRead(s.good()),
            reader(lineDelimiter, valueDelimiter, escapeChar) { ++(*this); }

    explicit CSVIterator(std::istream& s, bool isEnd) : is(s), canRead(!isEnd), reader() {}

    CSVIterator& operator++()
    {
        canRead = reader.CanRead(is);
        if (canRead)
        {
            reader.ReadIntoTuple(is, tuple);
        }
        return *this;
    }

    CSVIterator operator++(int)
    {
        CSVIterator t(*this);
        ++(*this);
        return t;
    }

    const std::tuple<Args...>& operator*() const
    {
        return tuple;
    }

    const std::tuple<Args...>* operator->() const
    {
        return &tuple;
    }

    bool operator==(const CSVIterator& other) const
    {
        return this == &other || canRead == other.canRead;
    }

    bool operator!=(const CSVIterator& other) const
    {
        return !((*this) == other);
    }

private:
    bool canRead = false;
    std::istream& is;
    std::tuple<Args...> tuple;
    CSVReader<Args...> reader;
};

template<typename ... Args>
class CSVParser
{
private:
    std::istream& inputStream;
    const char lineDelim = 0;
    const char valueDelim = 0;
    const char escapeChar = 0;
    size_t skipLinesCount;

public:
    explicit CSVParser(std::istream& _istream,
                       size_t skipLinesCount = 0,
                       char lineDelimiter = '\n',
                       char valueDelimiter = ',',
                       char escapeChar = '\"') :
            inputStream(_istream),
            lineDelim(lineDelimiter),
            valueDelim(valueDelimiter),
            escapeChar(escapeChar),
            skipLinesCount(skipLinesCount) {}

    CSVIterator<Args...> begin() const
    {
        CSVIterator<Args...> it(inputStream, lineDelim, valueDelim, escapeChar, 0);
        size_t skipped = 0;
        while (skipped++ < skipLinesCount)
            it++;
        return it;
    }

    CSVIterator<Args...> end() const
    {
        return CSVIterator<Args...>(inputStream, true);
    }
};
