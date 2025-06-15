#ifndef BOARD_HPP
#define BOARD_HPP

#include "bitboard.hpp"
#include "piece.hpp"
#include <array> 

// Simple struct holding the state to restore
struct Undo {
    int    ep_square;
    bool   w_can_castle_k, w_can_castle_q;
    bool   b_can_castle_k, b_can_castle_q;
    PieceType captured;
};

// Board Struct 
struct Board {
    std::array<U64, 12> bitboards; // 12 bitboards for each piece type and color
    
    U64 bothOccupancy; // Bitboard for all occupied squares
    U64 whiteOccupancy; // Bitboard for white occupied squares
    U64 blackOccupancy; // Bitboard for black occupied squares

    Color sideToMove = WHITE; // By default, white to move (can be changed to black)
    int enPassantSquare = -1;
    bool w_can_castle_k = true; // White can castle kingside
    bool w_can_castle_q = true; // White can castle queenside
    bool b_can_castle_k = true; // Black can castle kingside
    bool b_can_castle_q = true; // Black can castle queenside

    // Set up the initial position of the board
    void init_startpos();

    // Recompute the occupancy bitboards
    void recompute_occupancy();

    // Check square attack 
    bool is_square_attacked(int sq, Color bySide) const;

    // Make and undo moves for legal move generation
    Undo make_move(const Move& move);
    void undo_move(const Move& move, const Undo& undo);
};

#endif // BOARD_HPP