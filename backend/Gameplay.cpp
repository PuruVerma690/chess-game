#include "Gameplay.h"
#include "stockfishForm.h"
#include <cctype>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;
int Gameplay::rowFromRank(char rank)
{
    return 8 - (rank - '0');
}

int Gameplay::columnFromFile(char file)
{
    return file - 'a';
}

bool Gameplay::isPathClear(const ChessBoard &chessBoard, int sourceRow,
                           int sourceColumn, int destinationRow,
                           int destinationColumn)
{
    const int rowStep = destinationRow > sourceRow ? 1 : destinationRow < sourceRow ? -1 : 0;
    const int columnStep = destinationColumn > sourceColumn ? 1 : destinationColumn < sourceColumn ? -1 : 0;
    int row = sourceRow + rowStep;
    int column = sourceColumn + columnStep;

    while (row != destinationRow || column != destinationColumn)
    {
        if (chessBoard.getPiece(row, column) != Piece::Empty)
            return false;
        row += rowStep;
        column += columnStep;
    }
    return true;
}

void Gameplay::removeCastlingRight(char right)
{
    const size_t position = castlingAvailability.find(right);
    if (position != string::npos)
        castlingAvailability.erase(position, 1);
}

void Gameplay::removeCastlingRights(int code, int row, int column)
{
    if (code == Piece::WhiteKing)
    {
        removeCastlingRight('K');
        removeCastlingRight('Q');
    }
    else if (code == Piece::BlackKing)
    {
        removeCastlingRight('k');
        removeCastlingRight('q');
    }
    else if (code == Piece::WhiteRook)
    {
        if (row == 7 && column == 0)
            removeCastlingRight('Q');
        else if (row == 7 && column == 7)
            removeCastlingRight('K');
    }
    else if (code == Piece::BlackRook)
    {
        if (row == 0 && column == 0)
            removeCastlingRight('q');
        else if (row == 0 && column == 7)
            removeCastlingRight('k');
    }
}

void Gameplay::setupBoard()
{
    const int whiteBackRank[] = {
        Piece::WhiteRook, Piece::WhiteKnight, Piece::WhiteBishop,
        Piece::WhiteQueen, Piece::WhiteKing, Piece::WhiteBishop,
        Piece::WhiteKnight, Piece::WhiteRook};
    const int blackBackRank[] = {
        Piece::BlackRook, Piece::BlackKnight, Piece::BlackBishop,
        Piece::BlackQueen, Piece::BlackKing, Piece::BlackBishop,
        Piece::BlackKnight, Piece::BlackRook};

    for (int column = 0; column < 8; ++column)
    {
        board.setPiece(0, column, blackBackRank[column]);
        board.setPiece(1, column, Piece::BlackPawn);
        board.setPiece(6, column, Piece::WhitePawn);
        board.setPiece(7, column, whiteBackRank[column]);
    }
}

Gameplay::Gameplay()
{
    setupBoard();
}

bool Gameplay::playMove(const string &notation)
{
    if (notation.size() != 4)
        return false;

    const char destinationFile =
        static_cast<char>(tolower(static_cast<unsigned char>(notation[2])));
    const char destinationRank = notation[3];
    if (destinationFile < 'a' || destinationFile > 'h' ||
        destinationRank < '1' || destinationRank > '8')
        return false;

    const int destinationRow = rowFromRank(destinationRank);
    const int destinationColumn = columnFromFile(destinationFile);
    const char sourceFile =
        static_cast<char>(tolower(static_cast<unsigned char>(notation[0])));
    const char sourceRank = notation[1];
    if (sourceFile < 'a' || sourceFile > 'h' ||
        sourceRank < '1' || sourceRank > '8')
        return false;

    const int sourceRow = rowFromRank(sourceRank);
    const int sourceColumn = columnFromFile(sourceFile);
    const int player = whiteTurn ? 1 : 2;
    const int sourceCode = board.getPiece(sourceRow, sourceColumn);
    if (sourceCode / 10 != player)
        return false;

    const char requestedPiece = Piece::letter(sourceCode);
    const int rowDistance = abs(destinationRow - sourceRow);
    const int columnDistance = abs(destinationColumn - sourceColumn);
    bool canMove = false;

    if (requestedPiece == 'P')
    {
        const int direction = whiteTurn ? -1 : 1;
        const int startingRow = whiteTurn ? 6 : 1;
        const bool emptyDestination =
            board.getPiece(destinationRow, destinationColumn) ==
            Piece::Empty;
        canMove = sourceColumn == destinationColumn &&
                  destinationRow - sourceRow == direction && emptyDestination;
        canMove = canMove ||
                  (sourceColumn == destinationColumn && sourceRow == startingRow &&
                   destinationRow - sourceRow == 2 * direction &&
                   board.getPiece(sourceRow + direction, sourceColumn) ==
                       Piece::Empty &&
                   emptyDestination);
        canMove = canMove ||
                  (rowDistance == 1 && columnDistance == 1 &&
                   destinationRow - sourceRow == direction &&
                   board.getPiece(destinationRow, destinationColumn) !=
                       Piece::Empty &&
                   board.getPiece(destinationRow, destinationColumn) /
                           10 != player);
    }
    else if (requestedPiece == 'N')
        canMove = (rowDistance == 2 && columnDistance == 1) ||
                  (rowDistance == 1 && columnDistance == 2);
    else if (requestedPiece == 'K')
        canMove = rowDistance <= 1 && columnDistance <= 1 &&
                  (rowDistance + columnDistance > 0);
    else if (requestedPiece == 'B')
        canMove = rowDistance == columnDistance &&
                  isPathClear(board, sourceRow, sourceColumn, destinationRow,
                              destinationColumn);
    else if (requestedPiece == 'R')
        canMove = (rowDistance == 0 || columnDistance == 0) &&
                  isPathClear(board, sourceRow, sourceColumn, destinationRow,
                              destinationColumn);
    else if (requestedPiece == 'Q')
        canMove = (rowDistance == columnDistance || rowDistance == 0 ||
                   columnDistance == 0) &&
                  isPathClear(board, sourceRow, sourceColumn, destinationRow,
                              destinationColumn);

    const int destinationCode =
        board.getPiece(destinationRow, destinationColumn);
    if (canMove && destinationCode != Piece::Empty &&
        destinationCode / 10 == player)
        canMove = false;

    if (!canMove)
        return false;

    const int movingPiece = board.getPiece(sourceRow, sourceColumn);
    const int capturedPiece =
        board.getPiece(destinationRow, destinationColumn);
    removeCastlingRights(movingPiece, sourceRow, sourceColumn);
    removeCastlingRights(capturedPiece, destinationRow,
                         destinationColumn);
    board.setPiece(destinationRow, destinationColumn, movingPiece);
    board.setPiece(sourceRow, sourceColumn, Piece::Empty);

    if (requestedPiece == 'P' || capturedPiece != Piece::Empty)
        halfmoveClock = 0;
    else
        ++halfmoveClock;

    whiteTurn = !whiteTurn;
    if (whiteTurn)
        ++fullmoveNumber;
    return true;
}

void Gameplay::playGame(const vector<string> &moves)
{
    for (const string &move : moves)
    {
        if (!playMove(move))
            break;
    }
}

void Gameplay::printBoard() const
{
    cout << "\n  a b c d e f g h\n";
    for (int row = 0; row < 8; ++row)
    {
        cout << 8 - row << ' ';
        for (int column = 0; column < 8; ++column)
        {
            const int code = board.getPiece(row, column);
            char symbol = '.';
            if (code != Piece::Empty)
                symbol = Piece::tolower(code);
            cout << symbol << ' ';
        }
        cout << 8 - row << '\n';
    }
    cout << "  a b c d e f g h\n\n";
}

bool Gameplay::isWhiteTurn() const
{
    return whiteTurn;
}

string Gameplay::getStockfishForm() const
{
    return stockfishForm(board, whiteTurn, castlingAvailability,
                         halfmoveClock, fullmoveNumber)
        .toFEN();
}
