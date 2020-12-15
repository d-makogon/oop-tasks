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
                            Console::IntToChar(0), Console::IntToChar(maxX), 0, maxY);
    std::string s = Console::GetLine();
    if (s.length() < 2) return false;

    int x = Console::CharToInt(s[0]);
    int y;
    if (!str2T(s.substr(1), y)) return false;
    if (x < 0 || x > maxX || y < 0 || y > maxY) return false;
    coordinate = {x, y};
    return true;
}

bs::ShipType ChooseShipType(const std::vector<std::pair<bs::ShipType, int>>& availableTypes)
{
    Console::PrintLine("Choose one of available ship types: ");

    int curOption = 1;

    for (auto& type : availableTypes)
    {
        Console::PrintFormatted("%d - %s : %d cells : %d left\n", curOption, bs::ShipNames.at(type.first),
                                bs::ShipSizes.at(type.first), type.second);
        curOption++;
    }

    int option;
    // todo: make sth like exit cmd
    while (!TryGetIntOption(1, curOption - 1, option))
    {
        Console::PrintError("Wrong option. Try again.\n");
    }

    Console::PrintOK("You chose %s type.\n", bs::ShipNames.at(availableTypes[option - 1].first));

    return availableTypes[option - 1].first;
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
        Console::PrintError("Wrong option. Try again.\n");
    }

    Console::PrintOK("You chose %s direction.\n", bs::ShipDirsNames.at(availableDirs[option - 1]));

    return availableDirs[option - 1];
}

bs::Coordinate ChooseShipCoord(int maxX, int maxY)
{
    Console::PrintLine("Choose coordinate for first cell: ");

    bs::Coordinate coord;
    while (!TryGetCoordinate(maxX, maxY, coord))
    {
        Console::PrintError("Wrong coordinate. Try again.\n");
    }

    Console::PrintOK("You chose (%c, %d) cell.\n", Console::IntToChar(coord.GetX()), coord.GetY());

    return coord;
}

bs::BoardShip
bs::ConsolePlayerController::GetShipPlaceInfo(const std::vector<std::pair<bs::ShipType, int>>& availableTypes,
                                              const std::vector<ShipDirection>& availableDirs,
                                              int maxXcoord,
                                              int maxYcoord)
{
    auto type = ChooseShipType(availableTypes);
    auto shipDir = ChooseShipDir(availableDirs);
    auto coord = ChooseShipCoord(maxXcoord, maxYcoord);

    return bs::BoardShip(coord, shipDir, type);
}

void bs::ConsolePlayerController::ReceiveShipPlaceResult(const bs::ShipPlacementResult& result)
{
    switch (result)
    {
        case ShipPlacementResult::MaxTypeAmountReached:
            Console::PrintError("\nMax amount of ships of this type reached.\n");
            break;
        case ShipPlacementResult::NotEnoughSpace:
            Console::PrintError("\nNot enough space.\n");
            break;
        case ShipPlacementResult::Overlap:
            Console::PrintError("\nNew ship overlaps another one.\n");
            break;
        case ShipPlacementResult::MaxAmountReached:
            Console::PrintError("\nMax ships amount reached.\n");
            break;
        case ShipPlacementResult::Success:
            Console::PrintOK("\nSuccessfully placed a ship.\n");
            break;
        default:
            break;
    }
}

bs::Coordinate
bs::ConsolePlayerController::GetShootPosition(const bs::Board& enemyBoard)
{
    Console::PrintLine("Choose shot destination: ");

    bs::Coordinate coord;
    while (!TryGetCoordinate(enemyBoard.xSize - 1, enemyBoard.ySize - 1, coord))
    {
        Console::PrintError("Wrong coordinate. Try again.\n");
    }

    Console::PrintOK("You chose (%c, %d).\n", Console::IntToChar(coord.GetX()), coord.GetY());
    return coord;
}

void bs::ConsolePlayerController::ReceiveAllyShotResult(const bs::ShotResult& shotResult)
{
    switch (shotResult)
    {
        case ShotResult::Hit:
            Console::PrintOK("\nYou hit a ship!\n");
            break;
        case ShotResult::Invalid:
            Console::PrintError("\nInvalid coordinate.\n");
            break;
        case ShotResult::Duplicate:
            Console::PrintError("\nYou already shot at this coordinate.\n");
            break;
        case ShotResult::Miss:
            Console::PrintInfo("\nYou missed.\n");
            break;
        case ShotResult::HitAndSunk:
            Console::PrintOK("\nYou sunk a ship!\n");
            break;
        case ShotResult::Victory:
            Console::PrintOK("\nYou sunk last ship and won the game!\n");
            break;
    }
}

void bs::ConsolePlayerController::ReceiveEnemyShotResult(const bs::Coordinate& coord, const bs::ShotResult& shotResult)
{
    Console::PrintColoredFormatted("Other player shot at %c%d.\n", Console::PrintStyle::ForegroundColor::Yellow,
                                   Console::IntToChar(coord.GetX()), coord.GetY());
    switch (shotResult)
    {
        case ShotResult::Invalid:
        case ShotResult::Duplicate:
        case ShotResult::Miss:
            Console::PrintInfo("\tThey missed\n");
            break;
        case ShotResult::Hit:
            Console::PrintInfo("\tThey hit a ship\n");
            break;
        case ShotResult::HitAndSunk:
            Console::PrintInfo("\tThey sunk a ship\n");
            break;
        default:
            break;
    }
}

bool bs::ConsolePlayerController::GetTrueOrFalse(const std::string& msg)
{
    while (true)
    {
        Console::PrintInfo(msg);
        Console::Print(" (y/n): ");
        auto s = Console::GetLine();
        if (s == "y") return true;
        if (s == "n") return false;
        Console::PrintError("Wrong option. Try again.\n");
    }
}
