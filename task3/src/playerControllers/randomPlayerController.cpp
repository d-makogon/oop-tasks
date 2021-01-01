#include "include/randomPlayerController.h"

#include <random.h>

bs::BoardShip
bs::RandomPlayerController::GetShipPlaceInfo(const bs::Board& board)
{
    static std::vector<bs::ShipDirection> dirOptions;
    if (dirOptions.empty())
        for (auto&[dir, name] : ShipDirsNames)
            dirOptions.push_back(dir);

    return random.GetRandomBoardShip(board.GetAvailableShipsAmount(), dirOptions, board.xSize - 1, board.ySize - 1);
}

bs::Coordinate
bs::RandomPlayerController::GetShootPosition(const bs::Board& enemyBoard)
{
    return random.GetRandomCoordinate(enemyBoard.xSize - 1, enemyBoard.ySize - 1);
}
