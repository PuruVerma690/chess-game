#include <iostream>
#include <cctype>
#include <string>
#include <vector>
using namespace std;

class ChessPiece
{
private:
    int code;

public:
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

    ChessPiece(int pieceCode = Empty) : code(pieceCode) {}

    int getCode() const
    {
        return code;
    }

    void setCode(int pieceCode)
    {
        code = pieceCode;
    }
};

class ChessBoard
{
private:
    vector<vector<ChessPiece>> board;

public:
    ChessBoard() : board(8, vector<ChessPiece>(8)) {}

    const vector<vector<ChessPiece>> &getBoard() const
    {
        return board;
    }

    ChessPiece getPiece(int row, int column) const
    {
        return board[row][column];
    }

    void setPiece(int row, int column, ChessPiece piece)
    {
        board[row][column] = piece;
    }
};

class Gameplay
{
private:
    ChessBoard board;
    bool whiteTurn = true;

    static int rowFromRank(char rank)
    {
        return 8 - (rank - '0');
    }

    static int columnFromFile(char file)
    {
        return file - 'a';
    }

    static char pieceLetter(int code)
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

    void setupBoard()
    {
        const int whiteBackRank[] = {
            ChessPiece::WhiteRook, ChessPiece::WhiteKnight,
            ChessPiece::WhiteBishop, ChessPiece::WhiteQueen,
            ChessPiece::WhiteKing, ChessPiece::WhiteBishop,
            ChessPiece::WhiteKnight, ChessPiece::WhiteRook};
        const int blackBackRank[] = {
            ChessPiece::BlackRook, ChessPiece::BlackKnight,
            ChessPiece::BlackBishop, ChessPiece::BlackQueen,
            ChessPiece::BlackKing, ChessPiece::BlackBishop,
            ChessPiece::BlackKnight, ChessPiece::BlackRook};

        for (int column = 0; column < 8; ++column)
        {
            board.setPiece(0, column, ChessPiece(blackBackRank[column]));
            board.setPiece(1, column, ChessPiece(ChessPiece::BlackPawn));
            board.setPiece(6, column, ChessPiece(ChessPiece::WhitePawn));
            board.setPiece(7, column, ChessPiece(whiteBackRank[column]));
        }
    }

public:
    Gameplay()
    {
        setupBoard();
    }

    bool playMove(const string &notation)
    {
        if (notation.size() < 2)
            return false;

        const char destinationFile = tolower(notation[notation.size() - 2]);
        const char destinationRank = notation[notation.size() - 1];
        if (destinationFile < 'a' || destinationFile > 'h' ||
            destinationRank < '1' || destinationRank > '8')
            return false;

        const int destinationRow = rowFromRank(destinationRank);
        const int destinationColumn = columnFromFile(destinationFile);
        const char requestedPiece = notation.size() == 2 ? 'P' : toupper(notation[0]);
        const int player = whiteTurn ? 1 : 2;
        int sourceRow = -1;
        int sourceColumn = -1;

        for (int row = 0; row < 8 && sourceRow == -1; ++row)
        {
            for (int column = 0; column < 8; ++column)
            {
                const int code = board.getPiece(row, column).getCode();
                if (code / 10 != player || pieceLetter(code) != requestedPiece)
                    continue;

                const int rowDistance = abs(destinationRow - row);
                const int columnDistance = abs(destinationColumn - column);
                bool canMove = false;

                if (requestedPiece == 'P')
                {
                    const int direction = whiteTurn ? -1 : 1;
                    const int startingRow = whiteTurn ? 6 : 1;
                    canMove = column == destinationColumn &&
                              destinationRow - row == direction &&
                              board.getPiece(destinationRow, destinationColumn).getCode() == ChessPiece::Empty;
                    canMove = canMove || (column == destinationColumn && row == startingRow &&
                                          destinationRow - row == 2 * direction &&
                                          board.getPiece(row + direction, column).getCode() == ChessPiece::Empty &&
                                          board.getPiece(destinationRow, destinationColumn).getCode() == ChessPiece::Empty);
                }
                else if (requestedPiece == 'N')
                    canMove = (rowDistance == 2 && columnDistance == 1) ||
                              (rowDistance == 1 && columnDistance == 2);
                else if (requestedPiece == 'K')
                    canMove = rowDistance <= 1 && columnDistance <= 1;
                else if (requestedPiece == 'B')
                    canMove = rowDistance == columnDistance;
                else if (requestedPiece == 'R')
                    canMove = rowDistance == 0 || columnDistance == 0;
                else if (requestedPiece == 'Q')
                    canMove = rowDistance == columnDistance || rowDistance == 0 || columnDistance == 0;

                if (canMove)
                {
                    sourceRow = row;
                    sourceColumn = column;
                    break;
                }
            }
        }

        if (sourceRow == -1)
            return false;

        const ChessPiece movingPiece = board.getPiece(sourceRow, sourceColumn);
        board.setPiece(destinationRow, destinationColumn, movingPiece);
        board.setPiece(sourceRow, sourceColumn, ChessPiece());
        whiteTurn = !whiteTurn;
        return true;
    }

    void playGame(const vector<string> &moves)
    {
        for (const string &move : moves)
        {
            if (!playMove(move))
                break;
        }
    }

    const vector<vector<ChessPiece>> &getBoard() const
    {
        return board.getBoard();
    }
};

int main()
{
    Gameplay gameplay;
    gameplay.playGame({"e4", "e5", "Nf3", "Nc6"});
    return 0;
}
