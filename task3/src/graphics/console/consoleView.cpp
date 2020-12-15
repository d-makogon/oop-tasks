#include "consoleView.h"

#include <iostream>
#include <functional>
#include <map>

const bs::ConsoleView::CellStylesMap bs::ConsoleView::CellStyles = {
        {CellChar::Hit,        {
                                       Console::PrintStyle::ForegroundColor::Green,
                                       Console::PrintStyle::BackgroundColor::Black,
                                       Console::PrintStyle::TextStyle::Bold,
                               }},
        {CellChar::HitAndSunk, {
                                       Console::PrintStyle::ForegroundColor::Green,
                                       Console::PrintStyle::BackgroundColor::Black,
                                       Console::PrintStyle::TextStyle::Bold,
                               }},
        {CellChar::Ship,       {
                                       Console::PrintStyle::ForegroundColor::Cyan,
                                       Console::PrintStyle::BackgroundColor::Black,
                                       Console::PrintStyle::TextStyle::Bold,
                               }},
        {CellChar::Miss,       {
                                       Console::PrintStyle::ForegroundColor::Red,
                                       Console::PrintStyle::BackgroundColor::Black,
                                       Console::PrintStyle::TextStyle::Bold
                               }},
        {CellChar::Empty,      Console::DefaultPrintStyle},
};

static Console::PrintStyle stagePrintStyle = {Console::PrintStyle::ForegroundColor::Cyan,
                                              Console::PrintStyle::BackgroundColor::Default,
                                              Console::PrintStyle::TextStyle::Bold};
static Console::PrintStyle turnPrintStyle = {Console::PrintStyle::ForegroundColor::Green,
                                             Console::PrintStyle::BackgroundColor::Default,
                                             Console::PrintStyle::TextStyle::None};
static Console::PrintStyle::ForegroundColor boardColor = Console::PrintStyle::ForegroundColor::Cyan;

bs::ConsoleView::CellChar bs::ConsoleView::FromHistory(const bs::ShotHistory& shotHistory)
{
    switch (shotHistory)
    {
        case bs::ShotHistory::Miss:
            return CellChar::Miss;

        case bs::ShotHistory::Hit:
            return CellChar::Hit;

        case bs::ShotHistory::HitAndSunk:
            return CellChar::HitAndSunk;

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
        Console::Print(Console::IntToChar(x), " ");
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
            Console::PrintColored(static_cast<char>(cell), CellStyles.at(cell));
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

void bs::ConsoleView::Do()
{
    bool finished = false;
    bool skippedPlacing = false;

    while (!finished)
    {
        Console::Clear();
        auto state = logic->GetState();
        bool reveal = false;
        switch (state)
        {
            case GameState::P1_PlaceShip:
            case GameState::P2_PlaceShip:
            {
                reveal = (state == GameState::P1_PlaceShip) ? pc1->IsHuman() : pc2->IsHuman();

                Console::PrintColored("STAGE 1: PLACING SHIPS\n", stagePrintStyle);

                Console::PrintColoredFormatted("Player %d turn\n", turnPrintStyle,
                                               (state == GameState::P1_PlaceShip) ? 1 : 2);

                if (reveal)
                {
                    std::string msg = "Do you want to place ships automatically?";
                    bool yes = (state == GameState::P1_PlaceShip) ? pc1->GetTrueOrFalse(msg) : pc2->GetTrueOrFalse(msg);
                    if (yes)
                    {
                        PlaceShipsAutomatically(logic->GetAllyBoard());
                        continue;
                    }
                }

                const auto& curBoard = logic->GetAllyBoard();

                if (reveal)
                {
                    Console::PrintColored("\nYour board:\n", boardColor);
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
                reveal = (state == GameState::P1_Shoot) ? pc1->IsHuman() : pc2->IsHuman();

                Console::PrintColored("STAGE 2: SHOOTING\n", stagePrintStyle);
                Console::PrintColoredFormatted("Player %d turn\n", turnPrintStyle,
                                               (state == GameState::P1_Shoot) ? 1 : 2);

                const auto& enemyBoard = logic->GetEnemyBoard();
                if (reveal)
                {
                    Console::PrintColored("\nYour board:\n\n", boardColor);
                    PrintAllyBoard(logic->GetAllyBoard());
                    Console::PrintColored("\nYour opponent's board:\n\n", boardColor);
                    PrintEnemyBoard(enemyBoard);
                }

                Shoot((state == GameState::P1_Shoot) ? *pc1 : *pc2, enemyBoard,
                      (state == GameState::P1_Shoot) ? *pc2 : *pc1);

                if (reveal)
                {
                    PrintEnemyBoard(enemyBoard);
                }
                else if ((state = logic->GetState()) != GameState::P1_Win && state != GameState::P2_Win)
                {
                    Console::PrintColored("\n\nYour board:\n\n", boardColor);
                    PrintAllyBoard(enemyBoard);
                    Console::PrintInfo("\nPress Enter to continue\n");
                    Console::GetLine();
                }

                break;
            }
            case GameState::P1_Win:
            case GameState::P2_Win:
            {
                Console::PrintInfo("Player %d won the game!\n", (state == GameState::P1_Win) ? 1 : 2);
                finished = true;
                break;
            }
        }
        if (!finished && reveal)
            Console::PrintLine("Press Enter to go to the next step.");
        else if (finished)
            Console::PrintLine("Press Enter to exit");
        if (reveal)
        {
            Console::GetLine();
        }
        else if (!skippedPlacing)
        {
            Console::PrintLine("Your opponent is placing ships. Press Enter to skip.\n");
            Console::GetLine();
            skippedPlacing = true;
        }
    }
}

