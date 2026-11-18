#pragma once

#include <vector>

using namespace std;

namespace Piece
{
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

    inline char letter(int code)
    {
        switch (code % 10)
        {
        case 1: return 'P';
        case 2: return 'N';
        case 3: return 'B';
        case 4: return 'R';
        case 5: return 'Q';
        case 6: return 'K';
        default: return '.';
        }
    }

    inline char tolower(int code)
    {
        const char upper = letter(code);
        if (code / 10 == 2)
            return static_cast<char>(upper + ('a' - 'A'));
        return upper;
    }
}

class ChessBoard
{
private:
    vector<vector<int>> board;

public:
    ChessBoard();
    const vector<vector<int>> &getBoard() const;
    int getPiece(int row, int column) const;
    void setPiece(int row, int column, int piece);
};
