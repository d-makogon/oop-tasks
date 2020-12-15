#include "randomPlayerController.h"

#include "../utility/random.h"

bs::BoardShip
bs::RandomPlayerController::GetShipPlaceInfo(const std::vector<std::pair<bs::ShipType, int>>& availableTypes,
                                             const std::vector<ShipDirection>& availableDirs,
                                             int maxXcoord,
                                             int maxYcoord)
{
    return random.GetRandomBoardShip(availableTypes, availableDirs, maxXcoord, maxYcoord);
}

bs::Coordinate
bs::RandomPlayerController::GetShootPosition(const bs::Board& enemyBoard)
{
    return random.GetRandomCoordinate(enemyBoard.xSize - 1, enemyBoard.ySize - 1);
}
