#include "StockfishAPI.h"
#include "Gameplay.h"

#include <iostream>
#include <cstdio>

using namespace std;

#ifdef _WIN32
extern "C"
{
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

    char buffer[256];
    string response;
    while (fgets(buffer, sizeof(buffer), pipe))
        response += buffer;

    const string key = "\"moves\":\"";
    const size_t start = response.find(key);

    const size_t moveStart = start + key.size();
    const size_t moveEnd = response.find('"', moveStart);

    const string move = response.substr(moveStart, moveEnd - moveStart);

    return move.substr(0, 4);
}

bool StockfishAPI::makeAIMove(Gameplay &game)
{
    string fen = game.getStockfishForm();
    cout << "\nSending FEN to Stockfish: " << fen << "\n";

    string move = getAIMove(fen);

    if (!move.empty() && game.playMove(move))
    {
        cout << "Stockfish played: " << move << "\n\n";
        return true;
    }

    return false;
}
