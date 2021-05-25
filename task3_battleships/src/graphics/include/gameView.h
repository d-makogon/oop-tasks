#pragma once

#include <utility>

#include <board.h>
#include <gameLogic.h>
#include <playerController.h>
#include <random.h>

namespace bs
{
    class GameView
    {
    protected:
        Random rand;

        std::unique_ptr<GameLogic> logic;

        std::unique_ptr<PlayerController> pc1;
        std::unique_ptr<PlayerController> pc2;

        void PlaceShip(PlayerController& pc, const Board& board);

        void PlaceShipsAutomatically(const Board& board);

        void Shoot(PlayerController& pc, const Board& board, PlayerController& other);

        GameView(std::unique_ptr<GameLogic> logic, std::unique_ptr<PlayerController> pc1,
                 std::unique_ptr<PlayerController> pc2)
                : logic(std::move(logic)),
                  pc1(std::move(pc1)),
                  pc2(std::move(pc2)) {}

    public:
        virtual void Do() = 0;

        virtual ~GameView() = default;
    };
}
