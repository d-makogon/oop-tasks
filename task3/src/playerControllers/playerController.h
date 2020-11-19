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

        // virtual bs::ShipType ChooseShipType(const std::vector<ShipType>& availableTypes) const = 0;
        //
        // virtual bs::ShipDirection ChooseShipDir(const std::vector<ShipDirection>& availableDirs) const = 0;
        //
        // virtual bs::Coordinate ChooseShipCoord(const int& maxX, const int& maxY) const = 0;

        virtual ~PlayerController() = default;
    };
}
