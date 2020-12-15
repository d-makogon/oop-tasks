#pragma once

#include "playerController.h"

namespace bs
{
    class ConsolePlayerController : public PlayerController
    {
    public:
        BoardShip GetShipPlaceInfo(const std::vector<std::pair<bs::ShipType, int>>& availableTypes,
                                   const std::vector<ShipDirection>& availableDirs,
                                   int maxXcoord,
                                   int maxYcoord) override;

        void ReceiveShipPlaceResult(const ShipPlacementResult& result) override;

        Coordinate GetShootPosition(const bs::Board& enemyBoard) override;

        void ReceiveEnemyShotResult(const bs::Coordinate& coord, const ShotResult& shotResult) override;

        bool GetTrueOrFalse(const std::string& msg) override;

        bool IsHuman() const override { return true; }

        void ReceiveAllyShotResult(const ShotResult& shotResult) override;
    };
}
