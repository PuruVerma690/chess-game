#include "stockfishForm.h"
#include <cctype>
#include <sstream>
#include <string>
using namespace std;
stockfishForm::stockfishForm(const ChessBoard &chessBoard, bool whiteTurn,
                             const string &castling,
                             const string &enPassant, int halfmove,
                             int fullmove)
    : board(chessBoard), whiteToMove(whiteTurn),
      castlingAvailability(castling), enPassantTarget(enPassant),
      halfmoveClock(halfmove), fullmoveNumber(fullmove)
{
}

char stockfishForm::pieceLetter(int code)
{
    static const char letters[] = {' ', ' ', 'N', 'B', 'R', 'Q', 'K'};
    const int pieceType = code % 10;
    const char letter = pieceType == 1 ? 'P' : letters[pieceType];
    return code / 10 == 1
               ? letter
               : static_cast<char>(tolower(static_cast<unsigned char>(letter)));
}

string stockfishForm::toFEN() const
{
    ostringstream fen;

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
                fen << emptySquares;
                emptySquares = 0;
            }
            fen << pieceLetter(code);
        }

        if (emptySquares > 0)
            fen << emptySquares;
        if (row < 7)
            fen << '/';
    }

    fen << (whiteToMove ? " w " : " b ")
        << (castlingAvailability.empty() ? "-" : castlingAvailability)
        << ' ' << (enPassantTarget.empty() ? "-" : enPassantTarget)
        << ' ' << halfmoveClock << ' ' << fullmoveNumber;
    return fen.str();
}

string stockfishForm::getFEN() const
{
    return toFEN();
}
