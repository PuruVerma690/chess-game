#include "stockfishForm.h"
#include <string>
using namespace std;
stockfishForm::stockfishForm(const ChessBoard &chessBoard, bool whiteTurn,
                             const string &castling,
                             int halfmove, int fullmove)
    : board(chessBoard), whiteToMove(whiteTurn),
      castlingAvailability(castling), halfmoveClock(halfmove),
      fullmoveNumber(fullmove)
{
}

string stockfishForm::toFEN() const
{
    string fen;

    for (int row = 0; row < 8; ++row)
    {
        int emptySquares = 0;
        for (int column = 0; column < 8; ++column)
        {
            const int code = board.getPiece(row, column);
            if (code == Piece::Empty)
            {
                ++emptySquares;
                continue;
            }

            if (emptySquares > 0)
            {
                fen += to_string(emptySquares);
                emptySquares = 0;
            }
            fen += Piece::tolower(code);
        }

        if (emptySquares > 0)
            fen += to_string(emptySquares);
        if (row < 7)
            fen += '/';
    }

    fen += whiteToMove ? " w " : " b ";
    fen += castlingAvailability.empty() ? "-" : castlingAvailability;
    fen += " - ";
    fen += to_string(halfmoveClock);
    fen += ' ';
    fen += to_string(fullmoveNumber);
    return fen;
}

string stockfishForm::getFEN() const
{
    return toFEN();
}
