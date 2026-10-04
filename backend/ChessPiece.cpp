#include "ChessPiece.h"

ChessPiece::ChessPiece(int pieceCode) : code(pieceCode)
{
}

int ChessPiece::getCode() const
{
    return code;
}

void ChessPiece::setCode(int pieceCode)
{
    code = pieceCode;
}
