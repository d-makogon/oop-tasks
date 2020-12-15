#include <iostream>
#include "optimalPlayerController.h"

std::vector<bs::Coordinate>
GetNeighbourCellsToShoot(const bs::Coordinate& coord, const bs::Board& board)
{
    int x = coord.GetX();
    int y = coord.GetY();

    // @formatter:off
    std::vector<bs::Coordinate> surCells = {
                            {x, y - 1},
            {x - 1, y},     {x, y},     {x + 1, y},
                            {x, y + 1}
    };
    // @formatter:on

    for (auto it = surCells.begin(); it != surCells.end();)
    {
        if (board.GetHistoryAt(*it) != bs::ShotHistory::Unknown
            || !board.IsValidCoordinate(*it))
        {
            surCells.erase(it);
        }
        else
        {
            it++;
        }
    }

    return surCells;
}

bs::Coordinate
bs::OptimalPlayerController::GetShootPosition(const bs::Board& enemyBoard)
{
    auto shotHistory = enemyBoard.GetShotHistory();
    for (auto&[coord, history] : shotHistory)
    {
        if (history == ShotHistory::Hit)
        {
            auto candidates = GetNeighbourCellsToShoot(coord, enemyBoard);
            if (!candidates.empty())
                return random.GetRandomInArray<Coordinate>(candidates);
        }
    }

    std::vector<Coordinate> availableCoords;
    availableCoords.reserve(enemyBoard.xSize * enemyBoard.ySize);

    for (int x = 0; x < enemyBoard.xSize; x++)
    {
        for (int y = 0; y < enemyBoard.ySize; y++)
        {
            Coordinate coord = {x, y};
            if (enemyBoard.GetHistoryAt(coord) != ShotHistory::Unknown)
                continue;
            auto surCells = enemyBoard.GetSurroundingCells(coord);
            bool add = true;
            for (auto& cell : surCells)
            {
                auto h = enemyBoard.GetHistoryAt(cell);
                if (h == ShotHistory::HitAndSunk)
                    add = false;
            }
            if (add) availableCoords.push_back(coord);
        }
    }

    return random.GetRandomInArray<Coordinate>(availableCoords);
}

bs::BoardShip
bs::OptimalPlayerController::GetShipPlaceInfo(const std::vector<std::pair<bs::ShipType, int>>& availableTypes,
                                              const std::vector<ShipDirection>& availableDirs, int maxXcoord,
                                              int maxYcoord)
{
    return bs::BoardShip({random.GetRandomInRange(0, maxXcoord),
                          random.GetRandomInRange(0, maxYcoord)},
                         random.GetRandomInArray<ShipDirection>(availableDirs),
                         random.GetRandomInArray<std::pair<ShipType, int>>(availableTypes).first);
}
