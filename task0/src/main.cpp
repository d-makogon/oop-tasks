#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>

template<class KeyT, class ValueT>
std::vector<pair<KeyT, ValueT>> sortMap(std::map<KeyT, ValueT>& M, bool (* cmp)(std::pair<KeyT, ValueT>&, std::pair<KeyT, ValueT>&))
{
    std::vector<pair<KeyT, ValueT>> v;
    v.reserve(M.size());

    for (auto& it : M)
    {
        v.push_back(it);
    }

    std::sort(v.begin(), v.end(), cmp);

    return v;
}

// counts words in istream separated by non-alphanumeric character
std::map<string, int> countWords(std::istream& input, int& wordsCount)
{
    std::string str;
    std::map<string, int> words;

    while (std::getline(input, str))
    {
        std::string word;
        std::size_t wordLength = 0;
        std::size_t length = str.length();
        for (std::size_t i = 0; i < length; i++)
        {
            bool alnum = std::isalnum(str[i]);
            if (alnum)
            {
                word.append(1, str[i]);
                wordLength++;
            }
            if (wordLength > 0 && (!alnum || i == length - 1))
            {
                wordsCount++;
                words[word] += 1;
                word.clear();
                wordLength = 0;
            }
        }
    }
    return words;
}

void printFrequencies(std::ostream& stream, std::vector<std::pair<std::string, int>>& words, int totalWords)
{
    for (auto& it : words)
    {
        float percent = it.second / (float)totalWords;
        stream << it.first << ',' << it.second << ',' << percent << endl;
    }
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru_RU.cp1251");

    if (argc != 3)
    {
        std::cout << "Wrong amount of arguments" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cout << "Error opening file " << argv[1] << std::endl;
        return 1;
    }

    std::ofstream outputFile(argv[2]);
    if (!outputFile.is_open())
    {
        std::cout << "Error opening file " << argv[2] << std::endl;
        return 1;
    }

    int totalWords = 0;
    std::map<std::string, int> words = countWords(inputFile, totalWords);

    auto sorted = sortMap<std::string, int>(words,
                                       [](std::pair<std::string, int>& a, std::pair<std::string, int>& b) { return a.second > b.second; });

    printFrequencies(outputFile, sorted, totalWords);

    return 0;
}
