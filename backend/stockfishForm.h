#pragma once

#include "ChessBoard.h"
#include <string>

class stockfishForm
{
private:
    const ChessBoard &board;
    bool whiteToMove;
    std::string castlingAvailability;
    std::string enPassantTarget;
    int halfmoveClock;
    int fullmoveNumber;

    static char pieceLetter(int code);

public:
    stockfishForm(const ChessBoard &chessBoard, bool whiteTurn = true,
                  const std::string &castling = "KQkq",
                  const std::string &enPassant = "-", int halfmove = 0,
                  int fullmove = 1);
    std::string toFEN() const;
    std::string getFEN() const;
};
