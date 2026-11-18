#include "Gameplay.h"
#include "StockfishAPI.h"
#include <iostream>
#include <string>
using namespace std;
int main()
{
    Gameplay game;
    string move;

    cout << "Chess game (vs Stockfish)\n";
    cout << "You are White. Black is played by Stockfish.\n";
    cout << "Enter moves such as e2e4 or g1f3.\n";
    cout << "Commands: board, fen, quit\n";

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
            if (move == "board")
                continue;
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
            // Black's turn: Send stockfishForm string to API and make the move in Gameplay
            if (!StockfishAPI::makeAIMove(game))
            {
                cout << "No legal moves left. Game ended.\n";
                break;
            }
        }
    }

    cout << "Game ended.\n";
    return 0;
}
