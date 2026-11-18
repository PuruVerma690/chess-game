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

char Gameplay::pieceLetter(int code)
{
    switch (code % 10)
    {
    case 2:
        return 'N';
    case 3:
        return 'B';
    case 4:
        return 'R';
    case 5:
        return 'Q';
    case 6:
        return 'K';
    default:
        return 'P';
    }
}

string Gameplay::squareName(int row, int column)
{
    string square;
    square += static_cast<char>('a' + column);
    square += static_cast<char>('8' - row);
    return square;
}

bool Gameplay::isPathClear(const ChessBoard &chessBoard, int sourceRow,
                           int sourceColumn, int destinationRow,
                           int destinationColumn)
{
    const int rowStep = (destinationRow > sourceRow) - (destinationRow < sourceRow);
    const int columnStep = (destinationColumn > sourceColumn) - (destinationColumn < sourceColumn);
    int row = sourceRow + rowStep;
    int column = sourceColumn + columnStep;

    while (row != destinationRow || column != destinationColumn)
    {
        if (chessBoard.getPiece(row, column).getCode() != ChessPiece::Empty)
            return false;
        row += rowStep;
        column += columnStep;
    }
    return true;
}

void Gameplay::removeCastlingRight(char right)
{
    const string::size_type position = castlingAvailability.find(right);
    if (position != string::npos)
        castlingAvailability.erase(position, 1);
}

void Gameplay::removeCastlingRights(int code, int row, int column)
{
    if (code == ChessPiece::WhiteKing)
    {
        removeCastlingRight('K');
        removeCastlingRight('Q');
    }
    else if (code == ChessPiece::BlackKing)
    {
        removeCastlingRight('k');
        removeCastlingRight('q');
    }
    else if (code == ChessPiece::WhiteRook)
    {
        if (row == 7 && column == 0)
            removeCastlingRight('Q');
        else if (row == 7 && column == 7)
            removeCastlingRight('K');
    }
    else if (code == ChessPiece::BlackRook)
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
        ChessPiece::WhiteRook, ChessPiece::WhiteKnight, ChessPiece::WhiteBishop,
        ChessPiece::WhiteQueen, ChessPiece::WhiteKing, ChessPiece::WhiteBishop,
        ChessPiece::WhiteKnight, ChessPiece::WhiteRook};
    const int blackBackRank[] = {
        ChessPiece::BlackRook, ChessPiece::BlackKnight, ChessPiece::BlackBishop,
        ChessPiece::BlackQueen, ChessPiece::BlackKing, ChessPiece::BlackBishop,
        ChessPiece::BlackKnight, ChessPiece::BlackRook};

    for (int column = 0; column < 8; ++column)
    {
        board.setPiece(0, column, ChessPiece(blackBackRank[column]));
        board.setPiece(1, column, ChessPiece(ChessPiece::BlackPawn));
        board.setPiece(6, column, ChessPiece(ChessPiece::WhitePawn));
        board.setPiece(7, column, ChessPiece(whiteBackRank[column]));
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
    const int sourceCode = board.getPiece(sourceRow, sourceColumn).getCode();
    if (sourceCode / 10 != player)
        return false;

    const char requestedPiece = pieceLetter(sourceCode);
    const int rowDistance = abs(destinationRow - sourceRow);
    const int columnDistance = abs(destinationColumn - sourceColumn);
    bool canMove = false;

    if (requestedPiece == 'P')
    {
        const int direction = whiteTurn ? -1 : 1;
        const int startingRow = whiteTurn ? 6 : 1;
        const bool emptyDestination =
            board.getPiece(destinationRow, destinationColumn).getCode() ==
            ChessPiece::Empty;
        canMove = sourceColumn == destinationColumn &&
                  destinationRow - sourceRow == direction && emptyDestination;
        canMove = canMove ||
                  (sourceColumn == destinationColumn && sourceRow == startingRow &&
                   destinationRow - sourceRow == 2 * direction &&
                   board.getPiece(sourceRow + direction, sourceColumn).getCode() ==
                       ChessPiece::Empty &&
                   emptyDestination);
        canMove = canMove ||
                  (rowDistance == 1 && columnDistance == 1 &&
                   destinationRow - sourceRow == direction &&
                   board.getPiece(destinationRow, destinationColumn).getCode() !=
                       ChessPiece::Empty &&
                   board.getPiece(destinationRow, destinationColumn).getCode() /
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
        board.getPiece(destinationRow, destinationColumn).getCode();
    if (canMove && destinationCode != ChessPiece::Empty &&
        destinationCode / 10 == player)
        canMove = false;

    if (!canMove)
        return false;

    const ChessPiece movingPiece = board.getPiece(sourceRow, sourceColumn);
    const ChessPiece capturedPiece =
        board.getPiece(destinationRow, destinationColumn);
    removeCastlingRights(movingPiece.getCode(), sourceRow, sourceColumn);
    removeCastlingRights(capturedPiece.getCode(), destinationRow,
                         destinationColumn);
    board.setPiece(destinationRow, destinationColumn, movingPiece);
    board.setPiece(sourceRow, sourceColumn, ChessPiece());

    enPassantTarget = "-";
    if (requestedPiece == 'P' && abs(destinationRow - sourceRow) == 2)
        enPassantTarget =
            squareName((sourceRow + destinationRow) / 2, sourceColumn);

    if (requestedPiece == 'P' || capturedPiece.getCode() != ChessPiece::Empty)
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
            const int code = board.getPiece(row, column).getCode();
            char symbol = '.';
            if (code != ChessPiece::Empty)
            {
                symbol = pieceLetter(code);
                if (code / 10 == 2)
                    symbol = static_cast<char>(
                        tolower(static_cast<unsigned char>(symbol)));
            }
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
                         enPassantTarget, halfmoveClock, fullmoveNumber)
        .toFEN();
}
