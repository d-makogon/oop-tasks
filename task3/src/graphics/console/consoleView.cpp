#include "consoleView.h"

#include <iostream>
#include <functional>
#include <map>

bs::ConsoleView::CellChar bs::ConsoleView::FromHistory(const bs::ShotHistory& shotHistory)
{
    switch (shotHistory)
    {
        case bs::ShotHistory::Miss:
            return CellChar::Miss;

        case bs::ShotHistory::Hit:
            return CellChar::Hit;

        default:
            return CellChar::Empty;
    }
}

void bs::ConsoleView::PrintBoard(int maxX, int maxY, const std::function<CellChar(const bs::Coordinate&)>& getCellFunc)
{
    // print row with X coordinates
    for (int x = 0; x < maxX; x++)
    {
        if (x == 0)
        {
            Console::Print("   ");
        }
        Console::Print(Console::CoordToLetter(x), " ");
    }
    Console::PrintLine();

    for (int y = 0; y < maxY; y++)
    {
        // print row number
        Console::Print(y, " |");

        // print row
        for (int x = 0; x < maxY; x++)
        {
            // print cell
            CellChar cell = getCellFunc({x, y});
            switch (cell)
            {
                // todo: map with all print parameters !
                case CellChar::Ship:
                    Console::PrintColored(static_cast<char>(cell),
                                          Console::ForegroundColor::Cyan,
                                          Console::BackgroundColor::Black,
                                          Console::TextStyle::Bold);
                    break;
                case CellChar::Hit:
                    Console::PrintColored(static_cast<char>(cell),
                                          Console::ForegroundColor::Green,
                                          Console::BackgroundColor::Black,
                                          Console::TextStyle::Bold);
                    break;
                case CellChar::Miss:
                    Console::PrintColored(static_cast<char>(cell),
                                          Console::ForegroundColor::Red,
                                          Console::BackgroundColor::Black,
                                          Console::TextStyle::Bold);
                    break;
                case CellChar::Empty:
                    Console::Print(" ");
                    break;
            }
            Console::Print("|");
        }
        Console::PrintLine();
    }
}

void bs::ConsoleView::PrintEnemyBoard(const bs::Board& board)
{
    PrintBoard(board.xSize, board.ySize,
               [board](const bs::Coordinate& coord) { return FromHistory(board.GetHistoryAt(coord)); });
}

void bs::ConsoleView::PrintAllyBoard(const bs::Board& board)
{
    PrintBoard(board.xSize, board.ySize, [board](const bs::Coordinate& coord)
    {
        auto history = board.GetHistoryAt(coord);
        if (history == bs::ShotHistory::Unknown)
        {
            return board.CheckForShip(coord) ? CellChar::Ship : CellChar::Empty;
        }
        return FromHistory(history);
    });
}

void bs::ConsoleView::Do(bool revealPlayer1, bool revealPlayer2)
{
    bool finished = false;
    while (!finished)
    {
        Console::Clear();
        auto state = logic->GetState();
        switch (state)
        {
            case GameState::P1_PlaceShip:
            case GameState::P2_PlaceShip:
            {
                bool reveal = (state == GameState::P1_PlaceShip) ? revealPlayer1 : revealPlayer2;

                Console::PrintColored("STAGE 1: PLACING SHIPS\n", Console::ForegroundColor::Cyan,
                                      Console::BackgroundColor::Black, Console::TextStyle::Bold);
                Console::PrintColoredFormatted("Player %d turn\n", Console::ForegroundColor::Green,
                                               Console::BackgroundColor::Black, Console::TextStyle::None,
                                               (state == GameState::P1_PlaceShip) ? 1 : 2);

                const auto& curBoard = logic->GetAllyBoard();

                if (reveal)
                {
                    Console::PrintColored("\nYour board:\n", Console::ForegroundColor::Cyan);
                    PrintAllyBoard(curBoard);
                    Console::PrintLine();
                }

                PlaceShip((state == GameState::P1_PlaceShip) ? *pc1 : *pc2, curBoard);

                if (reveal)
                {
                    PrintAllyBoard(curBoard);
                }
                break;
            }
            case GameState::P1_Shoot:
            case GameState::P2_Shoot:
            {
                bool reveal = (state == GameState::P1_Shoot) ? revealPlayer1 : revealPlayer2;

                Console::PrintColored("STAGE 2: SHOOTING\n", Console::ForegroundColor::Cyan,
                                      Console::BackgroundColor::Black, Console::TextStyle::Bold);
                Console::PrintColoredFormatted("Player %d turn\n", Console::ForegroundColor::Green,
                                               Console::BackgroundColor::Black, Console::TextStyle::None,
                                               (state == GameState::P1_Shoot) ? 1 : 2);

                const auto& enemyBoard = logic->GetEnemyBoard();
                if (reveal)
                {
                    Console::PrintColored("\nYour board:\n\n", Console::ForegroundColor::Yellow);
                    PrintAllyBoard(logic->GetAllyBoard());
                    Console::PrintColored("\nYour opponent's board:\n\n", Console::ForegroundColor::Cyan);
                    PrintEnemyBoard(enemyBoard);
                }
                Shoot((state == GameState::P1_Shoot) ? *pc1 : *pc2, enemyBoard);

                if (reveal)
                {
                    PrintEnemyBoard(enemyBoard);
                }

                break;
            }
            case GameState::P1_Win:
            case GameState::P2_Win:
            {
                Console::PrintFormatted("Player %d won the game!\n", (state == GameState::P1_Win) ? 1 : 2);
                finished = true;
                break;
            }
        }
        if (!finished)
            Console::PrintLine("Press Enter to go to the next step.");
        else
            Console::PrintLine("Press Enter to exit");
        Console::GetLine();
    }
}

