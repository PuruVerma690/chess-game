#include "Gameplay.h"
#include <iostream>
#include <string>

int main()
{
    Gameplay game;
    std::string move;

    std::cout << "Chess game\n";
    std::cout << "Enter moves such as e4, Nf3, or e2e4.\n";
    std::cout << "Commands: board, fen, quit\n";

    while (true)
    {
        game.printBoard();
        std::cout << (game.isWhiteTurn() ? "White" : "Black")
                  << " to move: ";

        if (!std::getline(std::cin, move))
            break;
        if (move == "quit" || move == "exit")
            break;
        if (move == "board")
            continue;
        if (move == "fen")
        {
            std::cout << game.getStockfishForm() << '\n';
            continue;
        }
        if (move.empty())
            continue;

        if (!game.playMove(move))
            std::cout << "Invalid move. Try again.\n";
    }

    std::cout << "Game ended.\n";
    return 0;
}
