#pragma once

#include "ChessBoard.h"
#include <string>

using namespace std;

class stockfishForm
{
private:
    const ChessBoard &board;
    bool whiteToMove;
    string castlingAvailability;
    int halfmoveClock;
    int fullmoveNumber;


public:
    stockfishForm(const ChessBoard &chessBoard, bool whiteTurn = true,
                  const string &castling = "KQkq",
                  int halfmove = 0, int fullmove = 1);
    string toFEN() const;
    string getFEN() const;
};
