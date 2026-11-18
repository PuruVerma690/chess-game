#include "ChessBoard.h"

ChessBoard::ChessBoard() : board(8, vector<int>(8, Piece::Empty))
{
}

const vector<vector<int>> &ChessBoard::getBoard() const
{
    return board;
}

int ChessBoard::getPiece(int row, int column) const
{
    return board.at(row).at(column);
}

void ChessBoard::setPiece(int row, int column, int piece)
{
    board.at(row).at(column) = piece;
}
