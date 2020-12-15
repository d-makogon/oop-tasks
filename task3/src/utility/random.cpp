#include "random.h"

bs::Coordinate Random::GetRandomCoordinate(int maxX, int maxY)
{
    return {GetRandomInRange(0, maxX), GetRandomInRange(0, maxY)};
}

bs::BoardShip Random::GetRandomBoardShip(const std::vector<std::pair<bs::ShipType, int>>& availableTypes,
                                         const std::vector<bs::ShipDirection>& availableDirs, int maxXcoord,
                                         int maxYcoord)
{
    return bs::BoardShip({GetRandomInRange(0, maxXcoord),
                          GetRandomInRange(0, maxYcoord)},
                         GetRandomInArray<bs::ShipDirection>(availableDirs),
                         GetRandomInArray<std::pair<bs::ShipType, int>>(availableTypes).first);
}
