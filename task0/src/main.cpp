#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;

template<class KeyT, class ValueT>
vector<pair<KeyT, ValueT>> sortMap(map<KeyT, ValueT>& M, bool (* cmp)(pair<KeyT, ValueT>&, pair<KeyT, ValueT>&))
{
    vector<pair<KeyT, ValueT>> v;
    v.reserve(M.size());

    for (auto& it : M)
    {
        v.push_back(it);
    }

    sort(v.begin(), v.end(), cmp);

    return v;
}

// counts words in istream separated by non-alphanumeric character
map<string, int> countWords(istream& input, int& wordsCount)
{
    string str;
    map<string, int> words;

    while (getline(input, str))
    {
        string word;
        size_t wordLength = 0;
        size_t length = str.length();
        for (size_t i = 0; i < length; i++)
        {
            bool alnum = isalnum(str[i]);
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

void printFrequencies(ostream& stream, vector<pair<string, int>>& words, int totalWords)
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
        cout << "Wrong amount of arguments" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        cout << "Error opening file " << argv[1] << endl;
        return 1;
    }

    ofstream outputFile(argv[2]);
    if (!outputFile.is_open())
    {
        cout << "Error opening file " << argv[2] << endl;
        return 1;
    }

    int totalWords = 0;
    map<string, int> words = countWords(inputFile, totalWords);

    auto sorted = sortMap<string, int>(words,
                                       [](pair<string, int>& a, pair<string, int>& b) { return a.second > b.second; });

    printFrequencies(outputFile, sorted, totalWords);

    return 0;
}
