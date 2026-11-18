#include "Gameplay.h"
#include "StockfishAPI.h"
#include <iostream>
#include <string>
using namespace std;
int main()
{
    Gameplay game;
    string move;

    cout << "You vs Stockfish\n";
    cout << "White moves first\n";
    cout << "Enter moves such as e2e4 or g1f3.\n";
    cout << "use fen to get fen from\n";
    cout << "use quit or exit to exit the game\n";

    while (true)
    {
        game.printBoard();

        if (game.isWhiteTurn())
        {
            cout << "White to move: ";

            if (!getline(cin, move))
                break;
            if (!move.empty() && move.back() == '\r')
                move.pop_back();
            if (move == "quit" || move == "exit")
                break;
            if (move == "fen")
            {
                cout << game.getStockfishForm() << '\n';
                continue;
            }
            if (move.empty())
                continue;

            if (!game.playMove(move))
                cout << "Invalid move. Try again.\n";
        }
        else
        {
            if (StockfishAPI::makeAIMove(game))
                continue;

            cout << "No legal moves left. Game ended.\n";
            break;
        }
    }

    cout << "Game ended.\n";
    return 0;
}
