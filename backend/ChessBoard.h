#pragma once

#include "ChessPiece.h"
#include <vector>

class ChessBoard
{
private:
    std::vector<std::vector<ChessPiece>> board;

public:
    ChessBoard();
    const std::vector<std::vector<ChessPiece>> &getBoard() const;
    ChessPiece getPiece(int row, int column) const;
    void setPiece(int row, int column, ChessPiece piece);
};
