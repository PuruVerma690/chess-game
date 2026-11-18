# chess-game

## run project mac / linux
```sh
c++ -std=c++17 -Wall -Wextra -pedantic backend/ChessPiece.cpp backend/ChessBoard.cpp backend/stockfishForm.cpp backend/Gameplay.cpp backend/StockfishAPI.cpp backend/main.cpp -o chess-game.exe
.\chess-game.exe
```

## run project windows
```sh
cd "C:\Users\User\OneDrive\Desktop\chess-game-main"

c++ -std=c++17 -Wall -Wextra -pedantic ` backend\ChessPiece.cpp ` backend\ChessBoard.cpp ` backend\stockfishForm.cpp ` backend\Gameplay.cpp ` backend\StockfishAPI.cpp ` backend\main.cpp ` -o chess-game.exe

.\chess-game.exe
```

The game uses the public Lichess Cloud Evaluation API for online Stockfish
analysis. No local Stockfish executable or API key is required. An internet
connection is required when Black moves.

then 
```sh
.\chess-game.exe
```
## Build and run

From the project root, use the project build target so all implementation files
are linked:

```sh
make
./chess-game
```

Do not compile `backend/main.cpp` by itself; `Gameplay` is implemented in
`backend/Gameplay.cpp` and requires the other backend sources at link time.

The game runs in the terminal. Enter moves in either of these forms:

- `e4` or `Nf3`
- Coordinate notation such as `e2e4` or `g1f3`

Available commands are `board`, `fen`, and `quit`. The `fen` command prints
the current position in the six-field format understood by Stockfish.
