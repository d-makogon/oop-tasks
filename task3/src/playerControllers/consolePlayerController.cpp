#include "consolePlayerController.h"

#include "../graphics/console/consoleController.h"

// signed types
template<typename T>
bool str2T(const std::string& str, T& ret)
{
    using TBigger = long long;
    const char* s = str.c_str();
    char* end;
    long long conv;
    errno = 0;
    conv = strtoll(s, &end, 10);
    if ((errno == ERANGE && conv == std::numeric_limits<TBigger>::max()) ||
        conv > std::numeric_limits<T>::max())
    {
        return false;
    }
    if ((errno == ERANGE && conv == std::numeric_limits<TBigger>::min()) ||
        conv < std::numeric_limits<T>::min())
    {
        return false;
    }
    if (*s == '\0' || *end != '\0')
    {
        return false;
    }
    ret = conv;
    return true;
}

bool TryGetIntOption(int rangeStart, int rangeEnd, int& choice)
{
    if (rangeStart > rangeEnd)
    {
        throw std::invalid_argument("Range start was > range end.");
    }
    if (rangeStart < rangeEnd)
    {
        Console::PrintFormatted("Type option (%d-%d): ", rangeStart, rangeEnd);
    }
    else
    {
        Console::PrintFormatted("Type option (%d): ", rangeStart);
    }

    int input;
    const auto& readSuccess = std::cin >> input;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (readSuccess)
    {
        if (input >= rangeStart && input <= rangeEnd)
        {
            choice = input;
            return true;
        }
        return false;
    }
    return false;
}

bool TryGetCoordinate(int maxX, int maxY, bs::Coordinate& coordinate)
{
    Console::PrintFormatted("Type coordinate in format xy (%c <= x <= %c; %d <= y <= %d): ",
                            Console::CoordToLetter(0), Console::CoordToLetter(maxX), 0, maxY);
    std::string s = Console::GetLine();
    if (s.length() < 2) return false;

    int x = Console::LetterToCoord(s[0]);
    int y;
    if (!str2T(s.substr(1), y)) return false;
    if (x < 0 || x > maxX || y < 0 || y > maxY) return false;
    coordinate = {x, y};
    return true;
}

bs::ShipType ChooseShipType(const std::vector<bs::ShipType>& availableTypes)
{
    Console::PrintLine("Choose one of available ship types: ");

    int curOption = 1;

    for (auto& type : availableTypes)
    {
        Console::PrintFormatted("%d - %s : %d cells\n", curOption, bs::ShipNames.at(type), bs::ShipSizes.at(type));
        curOption++;
    }

    int option;
    // todo: make sth like exit cmd
    while (!TryGetIntOption(1, curOption - 1, option))
    {
        Console::PrintColored("Wrong option. Try again.\n", Console::ForegroundColor::Red);
    }

    Console::PrintColoredFormatted("You chose %s type.\n",
                                   Console::ForegroundColor::Green,
                                   Console::BackgroundColor::Black,
                                   Console::TextStyle::Bold,
                                   bs::ShipNames.at(availableTypes[option - 1]));

    return availableTypes[option - 1];
}

bs::ShipDirection ChooseShipDir(const std::vector<bs::ShipDirection>& availableDirs)
{
    Console::PrintLine("Choose direction: ");

    int curOption = 1;
    for (auto& dir : availableDirs)
    {
        Console::PrintFormatted("%d - %s\n", curOption, bs::ShipDirsNames.at(dir));
        curOption++;
    }

    int option;
    while (!TryGetIntOption(1, curOption - 1, option))
    {
        Console::PrintColored("Wrong option. Try again.\n", Console::ForegroundColor::Red);
    }

    Console::PrintColoredFormatted("You chose %s direction.\n",
                                   Console::ForegroundColor::Green,
                                   Console::BackgroundColor::Black,
                                   Console::TextStyle::Bold,
                                   bs::ShipDirsNames.at(availableDirs[option - 1]));

    return availableDirs[option - 1];
}

bs::Coordinate ChooseShipCoord(int maxX, int maxY)
{
    Console::PrintLine("Choose coordinate for first cell: ");

    bs::Coordinate coord;
    while (!TryGetCoordinate(maxX, maxY, coord))
    {
        Console::PrintColored("Wrong coordinate. Try again.\n", Console::ForegroundColor::Red);
    }

    Console::PrintColoredFormatted("You chose (%c, %d) cell.\n",
                                   Console::ForegroundColor::Green,
                                   Console::BackgroundColor::Black,
                                   Console::TextStyle::Bold,
                                   Console::CoordToLetter(coord.GetX()), coord.GetY());

    return coord;
}

bs::BoardShip bs::ConsolePlayerController::GetShipPlaceInfo(const std::vector<ShipType>& availableTypes,
                                                            const std::vector<ShipDirection>& availableDirs,
                                                            int maxXcoord,
                                                            int maxYcoord)
{
    return bs::BoardShip(ChooseShipCoord(maxXcoord, maxYcoord),
                         ChooseShipDir(availableDirs),
                         ChooseShipType(availableTypes));
}

void bs::ConsolePlayerController::ReceiveShipPlaceResult(const bs::ShipPlacementResult& result)
{
    switch (result)
    {
        case ShipPlacementResult::MaxTypeAmountReached:
            Console::PrintColored("\nMax amount of ships of this type reached.\n",
                                  Console::ForegroundColor::Red);
            break;
        case ShipPlacementResult::NotEnoughSpace:
            Console::PrintColored("\nNot enough space.\n",
                                  Console::ForegroundColor::Red);
            break;
        case ShipPlacementResult::Overlap:
            Console::PrintColored("\nNew ship overlaps another one.\n",
                                  Console::ForegroundColor::Red);
            break;
        case ShipPlacementResult::MaxAmountReached:
            Console::PrintColored("\nMax ships amount reached.\n",
                                  Console::ForegroundColor::Red);
            break;
        case ShipPlacementResult::Success:
            Console::PrintColored("\nSuccessfully placed a ship.\n",
                                  Console::ForegroundColor::Green);
            break;
        default:
            break;
    }
}

bs::Coordinate
bs::ConsolePlayerController::GetShootPosition(int maxX, int maxY, const std::map<Coordinate, ShotHistory>& shotHistory)
{
    Console::PrintLine("Choose shot destination: ");

    bs::Coordinate coord;
    while (!TryGetCoordinate(maxX, maxY, coord))
    {
        Console::PrintColored("Wrong coordinate. Try again.\n", Console::ForegroundColor::Red);
    }

    Console::PrintColoredFormatted("You chose (%c, %d).\n",
                                   Console::ForegroundColor::Green,
                                   Console::BackgroundColor::Black,
                                   Console::TextStyle::Bold,
                                   Console::CoordToLetter(coord.GetX()), coord.GetY());
    return coord;
}

void bs::ConsolePlayerController::ReceiveShotResult(const bs::ShotResult& shotResult)
{
    switch (shotResult)
    {
        case ShotResult::Hit:
            Console::PrintColored("\nYou hit a ship!\n",
                                  Console::ForegroundColor::Green);
            break;
        case ShotResult::Invalid:
            Console::PrintColored("\nInvalid coordinate.\n",
                                  Console::ForegroundColor::Red);
            break;
        case ShotResult::Duplicate:
            Console::PrintColored("\nYou already shot at this coordinate.\n",
                                  Console::ForegroundColor::Red);
            break;
        case ShotResult::Miss:
            Console::PrintColored("\nYou missed.\n",
                                  Console::ForegroundColor::Yellow);
            break;
        case ShotResult::HitAndSunk:
            Console::PrintColored("\nYou sunk a ship!\n",
                                  Console::ForegroundColor::Green);
            break;
        case ShotResult::Victory:
            Console::PrintColored("\nYou sunk last ship and won the game!\n",
                                  Console::ForegroundColor::Green);
            break;
    }
}
