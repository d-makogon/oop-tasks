#pragma once

#include <vector>
#include <map>

#include "../game/ships/shipTypes.h"
#include "../game/board/board.h"

namespace bs
{
    class PlayerController
    {
    public:
        virtual BoardShip GetShipPlaceInfo(const std::vector<std::pair<bs::ShipType, int>>& availableTypes,
                                           const std::vector<ShipDirection>& availableDirs,
                                           int maxXcoord,
                                           int maxYcoord) = 0;

        virtual void ReceiveShipPlaceResult(const ShipPlacementResult& result) = 0;

        virtual Coordinate
        GetShootPosition(const bs::Board& enemyBoard) = 0;

        virtual void ReceiveAllyShotResult(const ShotResult& shotResult) = 0;

        virtual void ReceiveEnemyShotResult(const bs::Coordinate& coord, const ShotResult& shotResult) = 0;

        virtual bool GetTrueOrFalse(const std::string& msg) = 0;

        virtual bool IsHuman() const = 0;

        virtual ~PlayerController() = default;
    };
}
