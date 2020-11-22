#pragma once

#include "playerController.h"

namespace bs
{
    class ConsolePlayerController : public PlayerController
    {
    public:
        BoardShip GetShipPlaceInfo(const std::vector<ShipType>& availableTypes,
                                   const std::vector<ShipDirection>& availableDirs,
                                   int maxXcoord,
                                   int maxYcoord) override;

        void ReceiveShipPlaceResult(const ShipPlacementResult& result) override;

        Coordinate GetShootPosition(int maxX, int maxY, const std::map<Coordinate, ShotHistory>& shotHistory) override;

        void ReceiveShotResult(const ShotResult& shotResult) override;
    };
}
