#pragma once

#include "ChessBoard.h"
#include <string>
#include <vector>

using namespace std;

class Gameplay
{
private:
    ChessBoard board;
    bool whiteTurn = true;
    string castlingAvailability = "KQkq";
    int halfmoveClock = 0;
    int fullmoveNumber = 1;

    static int rowFromRank(char rank);
    static int columnFromFile(char file);
    static bool isPathClear(const ChessBoard &board, int sourceRow,
                            int sourceColumn, int destinationRow,
                            int destinationColumn);
    void removeCastlingRight(char right);
    void removeCastlingRights(int code, int row, int column);
    void setupBoard();

public:
    Gameplay();
    bool playMove(const string &notation);
    void playGame(const vector<string> &moves);
    void printBoard() const;
    bool isWhiteTurn() const;
    string getStockfishForm() const;
};
