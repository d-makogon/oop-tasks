#include "include/consoleView.h"

#include <functional>
#include <map>

Console::PrintStyle stagePrintStyle = {Console::PrintStyle::ForegroundColor::Cyan,
                                       Console::PrintStyle::BackgroundColor::Default,
                                       Console::PrintStyle::TextStyle::Bold};
Console::PrintStyle turnPrintStyle = {Console::PrintStyle::ForegroundColor::Green,
                                      Console::PrintStyle::BackgroundColor::Default,
                                      Console::PrintStyle::TextStyle::None};
Console::PrintStyle::ForegroundColor boardColor = Console::PrintStyle::ForegroundColor::Cyan;

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

void bs::ConsoleView::Update()
{
    auto state = logic->GetState();
    switch (state)
    {
        case GameState::RoundStart:
            UpdateRoundStartScreen(true);
            logic->StartRound();
            p1SkippedPlacing = false;
            p2SkippedPlacing = false;
            p1AskedAutoPlace = false;
            p2AskedAutoPlace = false;
            break;
        case GameState::P1_PlaceShip:
            UpdateShipPlaceScreen(pc1->IsHuman());
            if (!p2SkippedPlacing && !pc1->IsHuman() && pc2->IsHuman())
            {
                Console::PrintLine("Player 1 is placing ships.\nPress Enter to skip.");
                Console::GetLine();
                p2SkippedPlacing = true;
            }
            break;
        case GameState::P2_PlaceShip:
            UpdateShipPlaceScreen(pc2->IsHuman());
            if (!p1SkippedPlacing && !pc2->IsHuman() && pc1->IsHuman())
            {
                Console::PrintLine("Player 2 is placing ships.\nPress Enter to skip.");
                Console::GetLine();
                p1SkippedPlacing = true;
            }
            break;
        case GameState::P1_Shoot:
            UpdateShootScreen(pc1->IsHuman());
            break;
        case GameState::P2_Shoot:
            UpdateShootScreen(pc2->IsHuman());
            break;
        case GameState::P1_WinRound:
        case GameState::P2_WinRound:
            UpdateRoundWinScreen(true);
            logic->StartRound();
            break;
        case GameState::P1_WinGame:
        case GameState::P2_WinGame:
            UpdateGameWinScreen(true);
            break;
    }
}

void bs::ConsoleView::UpdateRoundStartScreen(bool verbose)
{
    if (verbose)
    {
        Console::PrintInfo("Round %d/%d. Current score: Player 1 - %d\tPlayer 2 - %d\n",
                           logic->GetPlayedGamesCount(), logic->GetGamesCount(), logic->GetP1Score(),
                           logic->GetP2Score());
        Console::PrintLine("Press Enter to continue.");
        Console::GetLine();
    }
}

void bs::ConsoleView::UpdateShipPlaceScreen(bool verbose)
{
    auto state = logic->GetState();
    Console::PrintColored("STAGE 1: PLACING SHIPS\n", stagePrintStyle);

    Console::PrintColoredFormatted("Player %d turn\n", turnPrintStyle,
                                   (state == GameState::P1_PlaceShip) ? 1 : 2);

    bool askedAutoPlace = (state == GameState::P1_PlaceShip) ? p1AskedAutoPlace : p2AskedAutoPlace;

    if (verbose && !askedAutoPlace)
    {
        std::string msg = "Do you want to place ships automatically?";
        bool yes = (state == GameState::P1_PlaceShip) ? pc1->GetYesOrNo(msg) : pc2->GetYesOrNo(msg);
        if (yes)
        {
            PlaceShipsAutomatically(logic->GetAllyBoard());
            return;
        }
        if (state == GameState::P1_PlaceShip)
            p1AskedAutoPlace = true;
        else
            p2AskedAutoPlace = true;
    }

    const auto& curBoard = logic->GetAllyBoard();

    if (verbose)
    {
        Console::PrintColored("\nYour board:\n", boardColor);
        PrintAllyBoard(curBoard);
        Console::PrintLine();
    }

    PlaceShip((state == GameState::P1_PlaceShip) ? *pc1 : *pc2, curBoard);

    if (verbose)
    {
        PrintAllyBoard(curBoard);
        Console::PrintLine("\nPress Enter to continue.");
        Console::GetLine();
    }
}

void bs::ConsoleView::UpdateShootScreen(bool verbose)
{
    auto state = logic->GetState();
    Console::PrintColored("STAGE 2: SHOOTING\n", stagePrintStyle);
    Console::PrintColoredFormatted("Player %d turn\n", turnPrintStyle,
                                   (state == GameState::P1_Shoot) ? 1 : 2);

    const auto& enemyBoard = logic->GetEnemyBoard();
    if (verbose)
    {
        Console::PrintColored("\nYour board:\n\n", boardColor);
        PrintAllyBoard(logic->GetAllyBoard());
        Console::PrintColored("\nYour opponent's board:\n\n", boardColor);
        PrintEnemyBoard(enemyBoard);
    }

    Shoot((state == GameState::P1_Shoot) ? *pc1 : *pc2, enemyBoard,
          (state == GameState::P1_Shoot) ? *pc2 : *pc1);

    if (verbose)
    {
        PrintEnemyBoard(enemyBoard);
    }
    else if ((state = logic->GetState()) != GameState::P1_WinRound && state != GameState::P2_WinRound &&
             state != GameState::P1_WinGame && state != GameState::P2_WinGame)
    {
        Console::PrintColored("\n\nYour board:\n\n", boardColor);
        PrintAllyBoard(enemyBoard);
    }
    Console::PrintInfo("\nPress Enter to continue\n");
    Console::GetLine();
}

void bs::ConsoleView::UpdateRoundWinScreen(bool verbose)
{
    auto state = logic->GetState();
    Console::PrintInfo("Player %d won current round!\nScore: Player 1 - %d\tPlayer 2 - %d\n",
                       (state == GameState::P1_WinRound) ? 1 : 2, logic->GetP1Score(), logic->GetP2Score());
    Console::PrintInfo("\nPress Enter to continue\n");
    Console::GetLine();
}

void bs::ConsoleView::UpdateGameWinScreen(bool verbose)
{
    Console::PrintInfo("Player %d won the game!\nScore: Player 1 - %d\tPlayer 2 - %d\n",
                       (logic->GetState() == GameState::P1_WinGame) ? 1 : 2, logic->GetP1Score(), logic->GetP2Score());
    Console::PrintInfo("\nPress Enter to continue\n");
    Console::GetLine();
}

void bs::ConsoleView::Do()
{
    bool finished = false;

    while (!finished)
    {
        Console::Clear();

        auto state = logic->GetState();

        Update();

        finished = (state == GameState::P1_WinGame || state == GameState::P2_WinGame);
    }
}
