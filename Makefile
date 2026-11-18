CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic

TARGET := chess-game
SOURCES := \
	backend/ChessPiece.cpp \
	backend/ChessBoard.cpp \
	backend/stockfishForm.cpp \
	backend/Gameplay.cpp \
	backend/StockfishAPI.cpp \
	backend/main.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
