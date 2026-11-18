#include "StockfishAPI.h"
#include "Gameplay.h"

#include <iostream>
#include <cstdio>
#include <sstream>

using namespace std;

#ifdef _WIN32
extern "C" {
    FILE *_popen(const char *command, const char *mode);
    int _pclose(FILE *stream);
}
#else
#define _popen popen
#define _pclose pclose
#endif

namespace
{
    string urlEncode(const string &value)
    {
        const char hex[] = "0123456789ABCDEF";
        string encoded;
        for (unsigned char character : value)
        {
            if ((character >= 'a' && character <= 'z') ||
                (character >= 'A' && character <= 'Z') ||
                (character >= '0' && character <= '9') ||
                character == '-' || character == '_' || character == '.' ||
                character == '~')
            {
                encoded += static_cast<char>(character);
            }
            else
            {
                encoded += '%';
                encoded += hex[character >> 4];
                encoded += hex[character & 0x0F];
            }
        }
        return encoded;
    }
}

string StockfishAPI::getAIMove(const string &fen)
{
    const string url = "https://lichess.org/api/cloud-eval?fen=" +
                       urlEncode(fen) + "&multiPv=1";
    const string command = "curl.exe -sS --fail \"" + url + "\"";

    FILE *pipe = _popen(command.c_str(), "r");
    if (!pipe)
    {
        cerr << "[Error] Could not connect to the Lichess Cloud Evaluation API.\n";
        return "";
    }

    char buffer[256];
    string response;
    while (fgets(buffer, sizeof(buffer), pipe))
        response += buffer;
    const int exitCode = _pclose(pipe);
    if (exitCode != 0)
    {
        cerr << "[Error] Lichess has no cloud evaluation for this position or "
                "the request failed.\n";
        return "";
    }

    const size_t pos = response.find("\"moves\":\"");
    if (pos == string::npos)
    {
        cerr << "[Error] Lichess returned no Stockfish move.\n";
        return "";
    }

    stringstream ss(response.substr(pos + 9));
    string move;
    ss >> move;

    if (move.size() < 4 || move[0] < 'a' || move[0] > 'h' ||
        move[1] < '1' || move[1] > '8' || move[2] < 'a' ||
        move[2] > 'h' || move[3] < '1' || move[3] > '8')
        return "";

    return move.substr(0, 4);
}

bool StockfishAPI::makeAIMove(Gameplay &game)
{
    string fen = game.getStockfishForm();
    cout << "\n[Black AI] Sending FEN to Stockfish: " << fen << "\n";

    string move = getAIMove(fen);

    if (!move.empty() && game.playMove(move))
    {
        cout << "[Black AI] Stockfish played: " << move << "\n\n";
        return true;
    }

    return false;
}
