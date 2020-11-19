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
        enum class CellType : char
        {
            Ship = '*',
            Hit = 'H',
            Miss = 'M',
            Empty = ' ',
        };

        void PlaceShip(PlayerController* pc);

        static void PrintEnemyBoard(const bs::Board& board);

        static void PrintAllyBoard(const bs::Board& board);

        static void PrintBoard(int maxX, int maxY, const std::function<CellType(const bs::Coordinate&)>& getCellFunc);

        static CellType FromHistory(const bs::ShotHistory& shotHistory);

    public:
        explicit ConsoleView(GameLogic logic, PlayerController* pc1, PlayerController* pc2) : GameView(std::move(logic),
                                                                                                       pc1,
                                                                                                       pc2) {}

        void Do() override;
    };
}
