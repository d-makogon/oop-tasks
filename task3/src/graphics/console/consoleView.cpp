#include "consoleView.h"

#include <iostream>
#include <functional>
#include <map>

bs::ConsoleView::CellType bs::ConsoleView::FromHistory(const bs::ShotHistory& shotHistory)
{
    switch (shotHistory)
    {
        case bs::ShotHistory::Miss:
            return CellType::Miss;

        case bs::ShotHistory::Hit:
            return CellType::Hit;

        default:
            return CellType::Empty;
    }
}

void bs::ConsoleView::PrintBoard(int maxX, int maxY, const std::function<CellType(const bs::Coordinate&)>& getCellFunc)
{
    // todo: if board is > 10?
    // print row with X coordinates
    for (int x = 0; x < maxX; x++)
    {
        if (x == 0)
        {
            Console::Print("   ");
        }
        Console::Print(Console::CoordToLetter(x), " ");
    }
    Console::PrintLine();

    for (int y = 0; y < maxY; y++)
    {
        // print row number
        Console::Print(y, " |");

        // print row
        for (int x = 0; x < maxY; x++)
        {
            // print cell
            CellType cell = getCellFunc({x, y});
            Console::Print(static_cast<char>(cell), "|");
        }
        Console::PrintLine();
    }
}

void bs::ConsoleView::PrintEnemyBoard(const bs::Board& board)
{
    PrintBoard(board.xSize, board.ySize,
               [board](const bs::Coordinate& coord) { return FromHistory(board.GetHistoryAt(coord)); });
}

void bs::ConsoleView::PrintAllyBoard(const bs::Board& board)
{
    PrintBoard(board.xSize, board.ySize, [board](const bs::Coordinate& coord)
    {
        auto history = board.GetHistoryAt(coord);
        if (history == bs::ShotHistory::Unknown)
        {
            return board.CheckForShip(coord) ? CellType::Ship : CellType::Empty;
        }
        return FromHistory(history);
    });
}

void bs::ConsoleView::PlaceShip(PlayerController* pc)
{
    const auto& board = logic.GetBoard();


    std::vector<ShipType> availableTypes;
    for (auto&[type, count] : board.GetMaxShipsAmount())
    {
        if (count > 0) availableTypes.push_back(type);
    }


    static std::vector<ShipDirection> availableDirs;
    if (availableDirs.empty())
    {
        availableDirs.reserve(ShipDirsNames.size());
        for (auto&[dir, name] : ShipDirsNames)
        {
            availableDirs.push_back(dir);
        }
    }

    bs::BoardShip bs = pc->GetShipPlaceInfo(availableTypes, availableDirs, board.xSize - 1, board.ySize - 1);

    Console::PrintFormatted("Chosen %s type, %s dir, (%c, %d) cell\n",
                            ShipNames.at(bs.GetType()),
                            ShipDirsNames.at(bs.GetDirection()),
                            Console::CoordToLetter(bs.GetCoordinate().GetX()),
                            bs.GetCoordinate().GetY());

    pc->ReceiveShipPlaceResult(logic.PlaceShip(bs));
}

void bs::ConsoleView::Do()
{
    bool finished = false;
    while (!finished)
    {
        auto state = logic.GetState();
        switch (state)
        {
            case GameState::P1_PlaceShip:
            case GameState::P2_PlaceShip:
                Console::PrintFormatted("STAGE 1: Placing ships\nPlayer %d turn\n",
                                        (state == GameState::P1_PlaceShip) ? 1 : 2);
                PlaceShip((state == GameState::P1_PlaceShip) ? pc1 : pc2);
                PrintAllyBoard(logic.GetBoard());
                break;
            case GameState::P1_Shoot:
                break;
            case GameState::P2_Shoot:
                break;
            case GameState::P1_Win:
                finished = true;
                break;
            case GameState::P2_Win:
                finished = true;
                break;
        }
    }
}