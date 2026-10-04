#pragma once

class ChessPiece
{
private:
    int code;

public:
    enum Code
    {
        Empty = 0,
        WhitePawn = 11,
        WhiteKnight = 12,
        WhiteBishop = 13,
        WhiteRook = 14,
        WhiteQueen = 15,
        WhiteKing = 16,
        BlackPawn = 21,
        BlackKnight = 22,
        BlackBishop = 23,
        BlackRook = 24,
        BlackQueen = 25,
        BlackKing = 26
    };

    explicit ChessPiece(int pieceCode = Empty);
    int getCode() const;
    void setCode(int pieceCode);
};
