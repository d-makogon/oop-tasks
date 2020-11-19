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
    };
}
