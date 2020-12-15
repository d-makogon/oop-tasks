#pragma once

#include "playerController.h"
#include "../utility/random.h"

namespace bs
{
    class RandomPlayerController : public PlayerController
    {
    private:
        Random random;

    public:
        RandomPlayerController() : random() {}

        BoardShip GetShipPlaceInfo(const std::vector<std::pair<bs::ShipType, int>>& availableTypes,
                                   const std::vector<ShipDirection>& availableDirs,
                                   int maxXcoord,
                                   int maxYcoord) override;

        void ReceiveShipPlaceResult(const ShipPlacementResult& result) override {}

        Coordinate GetShootPosition(const bs::Board& enemyBoard) override;

        bool IsHuman() const override { return false; }

        void ReceiveAllyShotResult(const ShotResult& shotResult) override {}

        bool GetTrueOrFalse(const std::string& msg) override { return true; }

        void ReceiveEnemyShotResult(const bs::Coordinate& coord, const ShotResult& shotResult) override {}
    };
}
