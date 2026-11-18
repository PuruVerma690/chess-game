#pragma once

#include <string>

using namespace std;

class Gameplay;

class StockfishAPI
{
public:
    // Sends the FEN to Stockfish and returns the best move.
    static string getAIMove(const string &fen);

    // Retrieves Stockfish's move and makes Black play it.
    static bool makeAIMove(Gameplay &game);
};
