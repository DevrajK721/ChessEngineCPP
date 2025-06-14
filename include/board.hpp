#ifndef BOARD_HPP
#define BOARD_HPP

#include "bitboard.hpp"
#include "piece.hpp"
#include <array> 

// Board Struct 
struct Board {
    std::array<U64, 12> bitboards; // 12 bitboards for each piece type and color
    
    U64 bothOccupancy; // Bitboard for all occupied squares
    U64 whiteOccupancy; // Bitboard for white occupied squares
    U64 blackOccupancy; // Bitboard for black occupied squares

    // Set up the initial position of the board
    void init_startpos();

    // Recompute the occupancy bitboards
    void recompute_occupancy();
};

#endif // BOARD_HPP