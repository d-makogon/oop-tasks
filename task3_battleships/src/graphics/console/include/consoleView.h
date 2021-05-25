#pragma once

#include <utility>
#include <functional>
#include <map>

#include <gameView.h>
#include <console.h>

namespace bs
{
    class ConsoleView : public GameView
    {
    private:
        enum class CellChar : char
        {
            Ship = '*',
            Hit = 'H',
            HitAndSunk = 'X',
            Miss = 'M',
            Empty = ' ',
        };

        using CellStylesMap = std::map<CellChar, Console::PrintStyle>;

        const CellStylesMap CellStyles = {
                {CellChar::Hit,        {Console::PrintStyle::ForegroundColor::Green,
                                               Console::PrintStyle::BackgroundColor::Black,
                                               Console::PrintStyle::TextStyle::Bold,}},
                {CellChar::HitAndSunk, {Console::PrintStyle::ForegroundColor::Green,
                                               Console::PrintStyle::BackgroundColor::Black,
                                               Console::PrintStyle::TextStyle::Bold,}},
                {CellChar::Ship,       {Console::PrintStyle::ForegroundColor::Cyan,
                                               Console::PrintStyle::BackgroundColor::Black,
                                               Console::PrintStyle::TextStyle::Bold,}},
                {CellChar::Miss,       {Console::PrintStyle::ForegroundColor::Red,
                                               Console::PrintStyle::BackgroundColor::Black,
                                               Console::PrintStyle::TextStyle::Bold}},
                {CellChar::Empty,      Console::DefaultPrintStyle},
        };

        void PrintEnemyBoard(const bs::Board& board);

        void PrintAllyBoard(const bs::Board& board);

        void PrintBoard(int maxX, int maxY, const std::function<CellChar(const bs::Coordinate&)>& getCellFunc);

        static CellChar FromHistory(const bs::ShotHistory& shotHistory);

        void Update();

        void UpdateRoundStartScreen(bool verbose);

        bool p1SkippedPlacing = false;
        bool p2SkippedPlacing = false;
        bool p1AskedAutoPlace = false;
        bool p2AskedAutoPlace = false;

        void UpdateShipPlaceScreen(bool verbose);

        void UpdateShootScreen(bool verbose);

        void UpdateRoundWinScreen(bool verbose);

        void UpdateGameWinScreen(bool verbose);

    public:
        explicit ConsoleView(std::unique_ptr<GameLogic> logic, std::unique_ptr<PlayerController> pc1,
                             std::unique_ptr<PlayerController> pc2) : GameView(std::move(logic),
                                                                               std::move(pc1),
                                                                               std::move(pc2)) {}

        void Do() override;
    };
}
