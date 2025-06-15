#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include "attacks.hpp"

// Move Struct 
struct Move {
    int from; // Source square
    int to; // Destination square
    PieceType piece; // Promotion piece type (0 for none, 1 for knight, 2 for bishop, 3 for rook, 4 for queen)
    PieceType captured; // True if the move is a capture
    PieceType promotion; // Promotion piece type (0 for no promotion)
    bool is_double_push; // True if the move is a double pawn push
    bool is_en_passant; // True if the move is an en passant capture
    bool is_castle; // True if the move is a castling move
};

// Create alias for easier readability
using MoveList = std::vector<Move>;

// Generate all pseudo-legal moves for the current position
MoveList generate_pseudo_legal_moves(const Board &b, Color side);

// Remove illegal moves from the list of pseudo-legal moves
MoveList generate_legal_moves(const Board &b, Color side);

#endif // MOVEGEN_HPP