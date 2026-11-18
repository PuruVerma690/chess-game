#include "ChessBoard.h"

ChessBoard::ChessBoard() : board(8, std::vector<ChessPiece>(8))
{
}

const std::vector<std::vector<ChessPiece>> &ChessBoard::getBoard() const
{
    return board;
}

ChessPiece ChessBoard::getPiece(int row, int column) const
{
    return board.at(row).at(column);
}

void ChessBoard::setPiece(int row, int column, ChessPiece piece)
{
    board.at(row).at(column) = piece;
}
