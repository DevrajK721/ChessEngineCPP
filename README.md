# ChessEngineCPP
A Chess Engine written in C++ inclusive of the board setup and the engine itself. 

## Usage 
### MacOS 
1. Download GCC and CMake using Homebrew 
```shell 
brew install gcc cmake
```
2. Create build directory and build project
```shell 
mkdir build && cd build 
cmake .. 
cmake --build . 
```

3. Run `chess-cli`
```shell 
./chess-cli
```

## Documentation 
[Bitboard Notes](docs/bitboard.md)
[Pieces Notes](docs/piece.md)
[Board Notes](docs/board.md)
[Attacks Notes](docs/attacks.md)
