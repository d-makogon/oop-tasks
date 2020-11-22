#pragma once

#include <utility>
#include <functional>

#include "../gameView.h"
#include "consoleController.h"

namespace bs
{
    class ConsoleView : public GameView
    {
    private:
        enum class CellChar : char
        {
            Ship = '*',
            Hit = 'H',
            Miss = 'M',
            Empty = ' ',
        };

        static void PrintEnemyBoard(const bs::Board& board);

        static void PrintAllyBoard(const bs::Board& board);

        static void PrintBoard(int maxX, int maxY, const std::function<CellChar(const bs::Coordinate&)>& getCellFunc);

        static CellChar FromHistory(const bs::ShotHistory& shotHistory);

    public:
        explicit ConsoleView(std::unique_ptr<GameLogic> logic, std::unique_ptr<PlayerController> pc1,
                             std::unique_ptr<PlayerController> pc2) : GameView(std::move(logic),
                                                                               std::move(pc1),
                                                                               std::move(pc2)) {}

        void Do(bool revealPlayer1, bool revealPlayer2) override;
    };
}
