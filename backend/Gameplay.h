#pragma once

#include "ChessBoard.h"
#include <string>
#include <vector>

class Gameplay
{
private:
    ChessBoard board;
    bool whiteTurn = true;
    std::string castlingAvailability = "KQkq";
    std::string enPassantTarget = "-";
    int halfmoveClock = 0;
    int fullmoveNumber = 1;

    static int rowFromRank(char rank);
    static int columnFromFile(char file);
    static char pieceLetter(int code);
    static std::string squareName(int row, int column);
    static bool isPathClear(const ChessBoard &board, int sourceRow,
                            int sourceColumn, int destinationRow,
                            int destinationColumn);
    void removeCastlingRight(char right);
    void removeCastlingRights(int code, int row, int column);
    void setupBoard();

public:
    Gameplay();
    bool playMove(const std::string &notation);
    void playGame(const std::vector<std::string> &moves);
    void printBoard() const;
    bool isWhiteTurn() const;
    std::string getStockfishForm() const;
};
