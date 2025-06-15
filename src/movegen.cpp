#include "movegen.hpp"
#include <cstdint>

// Masks for double pawn pushes 
constexpr U64 Rank2 = 0x000000000000FF00ULL; // Mask for Rank 2 (for white pawns)
constexpr U64 Rank7 = 0x00FF000000000000ULL; // Mask for Rank 7 (for black pawns)

// Declare the knight and king attacks arrays
U64 knightAttacks[64];
U64 kingAttacks[64];
U64 pawnAttacks[2][64];

MoveList generate_pseudo_legal_moves(const Board &b, Color side) {
    init_attacks(); // Initialize attacks if not already done

    MoveList moves; // List to store generated moves

    // Occupancy bitboards relative to the side to move
    U64 ownOccupancy = (side == WHITE) ? b.whiteOccupancy : b.blackOccupancy;
    U64 oppOccupancy = (side == WHITE) ? b.blackOccupancy : b.whiteOccupancy;
    U64 bothOccupancy = b.bothOccupancy;

    // Pawn Moves 
    {
        U64 pawns = b.bitboards[board_index(side, PAWN)];

        if (side == WHITE) {
            // Single Pushes
            U64 sgl = (pawns << 8) & ~bothOccupancy; // Single push
            U64 tmp = sgl; // Temporary variable for processing single pushes
            while (tmp) {
                int to = pop_lsb(tmp);
                int from = to - 8;
                moves.push_back({from, to, PAWN, NO_PIECE, false}); // Single push move
            }
            
            // Double Pushes
            U64 dbl = (((pawns & Rank2) << 8) & ~bothOccupancy) << 8 & ~bothOccupancy; // Double push
            tmp = dbl; // Temporary variable for processing double pushes
            while (tmp) {
                int to = pop_lsb(tmp);
                int from = to - 16;
                moves.push_back({from, to, PAWN, NO_PIECE, true}); // Double push move
            }

            // Captures
            tmp = pawns;

            // Compute per square
            while (tmp) {
                int from = pop_lsb(tmp);
                U64 attacks = pawnAttacks[0][from] & oppOccupancy; // Get attacks for the pawn
                while (attacks) {
                    int to = pop_lsb(attacks);
                    moves.push_back({from, to, PAWN, oppOccupancy & (1ULL << to) ? NO_PIECE : NO_PIECE, false}); // Capture move
                }
            }
        } else {
            // Single Pushes
            U64 sgl = (pawns >> 8) & ~bothOccupancy; // Single push
            U64 tmp = sgl; // Temporary variable for processing single pushes
            while (tmp) {
                int to = pop_lsb(tmp);
                int from = to + 8;
                moves.push_back({from, to, PAWN, NO_PIECE, false}); // Single push move
            }
            
            // Double Pushes
            U64 dbl = (((pawns & Rank7) >> 8) & ~bothOccupancy) << 8 & ~bothOccupancy; // Double push
            tmp = dbl; // Temporary variable for processing double pushes
            while (tmp) {
                int to = pop_lsb(tmp);
                int from = to + 16;
                moves.push_back({from, to, PAWN, NO_PIECE, true}); // Double push move
            }

            // Captures
            tmp = pawns;

            // Compute per square
            while (tmp) {
                int from = pop_lsb(tmp);
                U64 attacks = pawnAttacks[1][from] & oppOccupancy; // Get attacks for the pawn
                while (attacks) {
                    int to = pop_lsb(attacks);
                    moves.push_back({from, to, PAWN, oppOccupancy & (1ULL << to) ? NO_PIECE : NO_PIECE, false}); // Capture move
                }
            }
        }
    }

    // Knight Moves 
    {
        U64 knights = b.bitboards[board_index(side, KNIGHT)];
        U64 tmp = knights; // Temporary variable for processing knight moves

        while (tmp) {
            int from = pop_lsb(tmp);
            U64 attacks = knightAttacks[from] & ~ownOccupancy; // Get knight attacks excluding own pieces
            U64 tmpAttacks = attacks; // Temporary variable for processing knight attacks
            while (tmpAttacks) {
                int to = pop_lsb(tmpAttacks);
                moves.push_back({from, to, KNIGHT, oppOccupancy & (1ULL << to) ? NO_PIECE: NO_PIECE, false}); // Knight move
            }
        }
    }

    // Bishop, Rook & Queen Moves
    auto do_sliding_moves = [&](PieceType pt, auto attack_fn) {
        U64 bb = b.bitboards[board_index(side, pt)];
        U64 tmp = bb; // Temporary variable for processing sliding piece moves
        while (tmp) {
            int from = pop_lsb(tmp);
            U64 attacks = attack_fn(from, bothOccupancy) & ~ownOccupancy; // Get attacks for the sliding piece
            U64 tmpAttacks = attacks; // Temporary variable for processing sliding piece attacks
            while (tmpAttacks) {
                int to = pop_lsb(tmpAttacks);
                moves.push_back({from, to, pt, oppOccupancy & (1ULL << to) ? NO_PIECE : NO_PIECE, false}); // Sliding piece move
            }
        }
    };

    do_sliding_moves(BISHOP, bishop_attacks);
    do_sliding_moves(ROOK, rook_attacks);
    do_sliding_moves(QUEEN, queen_attacks);

    // King Moves 
    {
        U64 king = b.bitboards[board_index(side, KING)];
        U64 tmp = king; // Temporary variable for processing king moves
        int from = pop_lsb(tmp); // Get the king's position
        U64 attacks = kingAttacks[from] & ~ownOccupancy; // Get king attacks excluding own pieces
        U64 tmpAttacks = attacks; // Temporary variable for processing king attacks
        while (tmpAttacks) {
            int to = pop_lsb(tmpAttacks);
            moves.push_back({from, to, KING, oppOccupancy & (1ULL << to) ? NO_PIECE : NO_PIECE, false}); // King move
        }
    }
    return moves; // Return the list of generated pseudo-legal moves
}