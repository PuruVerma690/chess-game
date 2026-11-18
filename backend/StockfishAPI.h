#pragma once

#include <string>

class Gameplay;

class StockfishAPI
{
public:
    // Sends the FEN to Stockfish and returns the best move.
    static std::string getAIMove(const std::string &fen);

    // Retrieves Stockfish's move and makes Black play it.
    static bool makeAIMove(Gameplay &game);
};
