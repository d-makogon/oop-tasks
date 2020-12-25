#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "include/tuplePrint.h"
#include "include/csvParser.h"

int main()
{
    std::ifstream ifs("in.csv");
    if (!ifs.is_open()) return 1;

    CSVParser<int, std::string, std::string, double, double, double, double, double, double, double, double, double, double, double, double, std::string, std::string, int>
            parser(
            ifs, 0);
    try
    {
        std::string header;
        std::getline(ifs, header);
        std::cout << header << std::endl;
        for (const auto& t : parser)
        {
            std::cout <<  t << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
