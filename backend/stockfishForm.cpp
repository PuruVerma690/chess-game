#include "stockfishForm.h"
#include <cctype>
#include <sstream>

stockfishForm::stockfishForm(const ChessBoard &chessBoard, bool whiteTurn,
                             const std::string &castling,
                             const std::string &enPassant, int halfmove,
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
               : static_cast<char>(std::tolower(static_cast<unsigned char>(letter)));
}

std::string stockfishForm::toFEN() const
{
    std::ostringstream fen;

    for (int row = 0; row < 8; ++row)
    {
        int emptySquares = 0;
        for (int column = 0; column < 8; ++column)
        {
            const int code = board.getPiece(row, column).getCode();
            if (code == ChessPiece::Empty)
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

std::string stockfishForm::getFEN() const
{
    return toFEN();
}
