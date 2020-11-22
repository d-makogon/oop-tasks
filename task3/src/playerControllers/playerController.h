#pragma once

#include <vector>

#include "../game/ships/shipTypes.h"
#include "../game/board/board.h"

namespace bs
{
    class PlayerController
    {
    public:
        virtual BoardShip GetShipPlaceInfo(const std::vector<ShipType>& availableTypes,
                                           const std::vector<ShipDirection>& availableDirs,
                                           int maxXcoord,
                                           int maxYcoord) = 0;

        virtual void ReceiveShipPlaceResult(const ShipPlacementResult& result) = 0;

        virtual Coordinate
        GetShootPosition(int maxX, int maxY, const std::map<Coordinate, ShotHistory>& shotHistory) = 0;

        virtual void ReceiveShotResult(const ShotResult& shotResult) = 0;

        virtual ~PlayerController() = default;
    };
}
