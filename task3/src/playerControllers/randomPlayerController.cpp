#include "randomPlayerController.h"

#include <random>

int bs::RandomPlayerController::GetRandomInRange(int min, int max)
{
    std::uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
}

bs::BoardShip bs::RandomPlayerController::GetShipPlaceInfo(const std::vector<ShipType>& availableTypes,
                                                           const std::vector<ShipDirection>& availableDirs,
                                                           int maxXcoord,
                                                           int maxYcoord)
{
    // todo: test random (is it truly random?)
    return bs::BoardShip({GetRandomInRange(0, maxXcoord), GetRandomInRange(0, maxYcoord)},
                         GetRandomInArray<ShipDirection>(availableDirs),
                         GetRandomInArray<ShipType>(availableTypes));
}

void bs::RandomPlayerController::ReceiveShipPlaceResult(const bs::ShipPlacementResult& result)
{

}

