#include "board.hpp"
#include "attacks.hpp"
#include "movegen.hpp"
#include <initializer_list>
#include <cassert>

// Helper function to set several bits at once 
template <size_t N> // Template to handle arrays of different sizes
static void set_bits(U64 &bb, const std::array<int, N>& squares) {
    for (int sq : squares) {
        set_bit(bb, sq);
    }
}

// Bitboard setup
void Board::init_startpos() {
    // Initialize all bitboards to zero 
    for (auto &bb : bitboards) {
        bb = 0ULL;
    }

    // White Pawns on Rank 2 
    std::array<int, 8> whitePawns = {sq_index('a', '2'), sq_index('b', '2'), sq_index('c', '2'),
                                     sq_index('d', '2'), sq_index('e', '2'), sq_index('f', '2'),
                                     sq_index('g', '2'), sq_index('h', '2')};
    int whitePawnsIndex = board_index(WHITE, PAWN);

    set_bits(bitboards[whitePawnsIndex], whitePawns);

    // Black Pawns on Rank 7
    std::array<int, 8> blackPawns = {sq_index('a', '7'), sq_index('b', '7'), sq_index('c', '7'),
                                     sq_index('d', '7'), sq_index('e', '7'), sq_index('f', '7'),
                                     sq_index('g', '7'), sq_index('h', '7')};
    int blackPawnsIndex = board_index(BLACK, PAWN);
    set_bits(bitboards[blackPawnsIndex], blackPawns);

    // White Knights on b1 and g1
    std::array<int, 2> whiteKnights = {sq_index('b', '1'), sq_index('g', '1')};
    int whiteKnightsIndex = board_index(WHITE, KNIGHT);
    set_bits(bitboards[whiteKnightsIndex], whiteKnights);

    // Black Knights on b8 and g8
    std::array<int, 2> blackKnights = {sq_index('b', '8'), sq_index('g', '8')};
    int blackKnightsIndex = board_index(BLACK, KNIGHT);
    set_bits(bitboards[blackKnightsIndex], blackKnights);

    // White Bishops on c1 and f1
    std::array<int, 2> whiteBishops = {sq_index('c', '1'), sq_index('f', '1')};
    int whiteBishopsIndex = board_index(WHITE, BISHOP);
    set_bits(bitboards[whiteBishopsIndex], whiteBishops);

    // Black Bishops on c8 and f8
    std::array<int, 2> blackBishops = {sq_index('c', '8'), sq_index('f', '8')};
    int blackBishopsIndex = board_index(BLACK, BISHOP);
    set_bits(bitboards[blackBishopsIndex], blackBishops);

    // White Rooks on a1 and h1
    std::array<int, 2> whiteRooks = {sq_index('a', '1'), sq_index('h', '1')};
    int whiteRooksIndex = board_index(WHITE, ROOK);
    set_bits(bitboards[whiteRooksIndex], whiteRooks);

    // Black Rooks on a8 and h8
    std::array<int, 2> blackRooks = {sq_index('a', '8'), sq_index('h', '8')};
    int blackRooksIndex = board_index(BLACK, ROOK);
    set_bits(bitboards[blackRooksIndex], blackRooks);

    // White Queen on d1
    int whiteQueenIndex = board_index(WHITE, QUEEN);
    set_bit(bitboards[whiteQueenIndex], sq_index('d', '1'));
    
    // Black Queen on d8
    int blackQueenIndex = board_index(BLACK, QUEEN);
    set_bit(bitboards[blackQueenIndex], sq_index('d', '8'));
    
    // White King on e1
    int whiteKingIndex = board_index(WHITE, KING);
    set_bit(bitboards[whiteKingIndex], sq_index('e', '1'));

    // Black King on e8
    int blackKingIndex = board_index(BLACK, KING);
    set_bit(bitboards[blackKingIndex], sq_index('e', '8'));

    // Update occupancy bitboards
    recompute_occupancy();

}

void Board::recompute_occupancy() {
    // Clear occupancy bitboards
    bothOccupancy = 0ULL;
    whiteOccupancy = 0ULL;
    blackOccupancy = 0ULL;

    // Recompute bothOccupancy, whiteOccupancy, and blackOccupancy
    for (int pt = PAWN; pt <= KING; pt++) {
        whiteOccupancy |= bitboards[board_index(WHITE, PieceType(pt))];
        blackOccupancy |= bitboards[board_index(BLACK, PieceType(pt))];  
    }

    bothOccupancy = whiteOccupancy | blackOccupancy;

}

bool Board::is_square_attacked(int sq, Color bySide) const {
    U64 occ = bothOccupancy; // Local copy of occupancy bitboard
    Color other = bySide == WHITE ? BLACK : WHITE; // Determine the opponent's color from who's turn it is

    // Check for pawn attacks 
    int pi = board_index(bySide, PAWN); // Get the index for the pawn for current side
    if (bySide == WHITE) {
        // Get pawn attacks from black pawns and compare to positions on white bitboards
        if (pawnAttacks[1][sq] & bitboards[pi]) {
            return true;
        } else {
            // Get pawn attacks from black pawns and compare to positions on white bitboards
            if (pawnAttacks[0][sq] & bitboards[pi]) {
                return true;
            }
        }
    }

    // Check for knight attacks 
    if (knightAttacks[sq] & bitboards[board_index(bySide, KNIGHT)]) {
        return true;
    }

    // Check for king attacks
    if (kingAttacks[sq] & bitboards[board_index(bySide, KING)]) {
        return true;
    }

    // Check for sliding piece attacks (Bishops, Rooks, Queens)
    auto occ64 = occ; // Local copy of occupancy bitboard as U64
    // Bishops/Queens 
    if (bishop_attacks(sq, occ64) & (bitboards[board_index(bySide, BISHOP)] | bitboards[board_index(bySide, QUEEN)])) {
        return true; 
    }
    // Rooks/Queens
    if (rook_attacks(sq, occ64) & (bitboards[board_index(bySide, ROOK)] | bitboards[board_index(bySide, QUEEN)])) {
        return true;
    }

    return false; // If no attacks found, return false

}

Undo Board::make_move(const Move& move) {
    Undo undo; // Undo struct to save the state 

    // Save undo information
    undo.ep_square = enPassantSquare;
    undo.w_can_castle_k = w_can_castle_k;
    undo.w_can_castle_q = w_can_castle_q;
    undo.b_can_castle_k = b_can_castle_k;
    undo.b_can_castle_q = b_can_castle_q;
    undo.captured = move.captured; 

    // Clear enPassant (will be reset if there is a double pawn push)
    enPassantSquare = -1;

    Color us = sideToMove; 
    Color them = (us == WHITE) ? BLACK : WHITE; 

    // Remove the piece from the source square
    int from = move.from; // Source square
    clear_bit(bitboards[board_index(us, move.piece)], from); // Clear the piece from the source square

    /*
    SPECIAL CASES:
    - En Passant Capture
    - Castling
    */
    
    // En Passant Capture 
    if (move.is_en_passant) {
        // Remove the pawn being captured 
        int capturedSquare = move.to + (us == WHITE ? -8 : 8); // Calculate the square of the captured pawn (file below to for white, file above for black)
        int capturedIndex = board_index(them, PAWN); // Get the index for the opponent's pawn (for the bitboard)
        clear_bit(bitboards[capturedIndex], capturedSquare); // Clear the captured pawn from the bitboard

    } else if (move.captured != NO_PIECE) {
        // Regular capture
        int capturedIndex = board_index(them, move.captured); // Get the index for the captured piece
        clear_bit(bitboards[capturedIndex], move.to); // Clear the captured piece from the bitboard
    }

    // Castling 
    if (move.is_castle) {
        if (us == WHITE) {
            // King goes e1->g1 (kingside) or e1->c1 (queenside)
            if (move.to == sq_index('g', '1')) {
                // Kingside: Rook moves h1->f1
                clear_bit(bitboards[board_index(WHITE, ROOK)], sq_index('h', '1'));
                set_bit(bitboards[board_index(WHITE, ROOK)], sq_index('f', '1')); // Place rook on f1
            } else {
                // Queenside: Rook moves a1->d1
                clear_bit(bitboards[board_index(WHITE, ROOK)], sq_index('a', '1'));
                set_bit(bitboards[board_index(WHITE, ROOK)], sq_index('d', '1')); // Place rook on d1
            }
        } else {
            // King goes e8->g8 (kingside) or e8->c8 (queenside)
            if (move.to == sq_index('g', '8')) {
                // Kingside: Rook moves h8->f8
                clear_bit(bitboards[board_index(BLACK, ROOK)], sq_index('h', '8'));
                set_bit(bitboards[board_index(BLACK, ROOK)], sq_index('f', '8')); // Place rook on f8
            } else {
                // Queenside: Rook moves a8->d8
                clear_bit(bitboards[board_index(BLACK, ROOK)], sq_index('a', '8'));
                set_bit(bitboards[board_index(BLACK, ROOK)], sq_index('d', '8')); // Place rook on d8
            }
        }
    }

    // Place the moving piece on to the 'to' square (or promote)
    PieceType movePiece = move.promotion != NO_PIECE ? move.promotion : move.piece; // Use promotion piece if applicable
    int to_idx = board_index(us, movePiece); // Get the index for the piece type being moved (dynamic based on promotion)
    set_bit(bitboards[board_index(us, move.piece)], move.to); // Set the piece on the destination square

    // Handle double pawn push 
    if (move.piece == PAWN && move.is_double_push) {
        // Set enPassantSquare to the square behind the pawn
        enPassantSquare = (move.from + move.to) / 2; // Calculate the square behind the pawn (works for white and black)
    }

    // Update castling rights (if a king or rook is moved or a rook is captured)
    if (move.piece == KING) {
        // Clear castling rights on both sides for the king
        if (us == WHITE) {
            w_can_castle_k = false;
            w_can_castle_q = false;
        } else {
            b_can_castle_k = false;
            b_can_castle_q = false;
        }
    } 

    if (move.piece == ROOK) {
        // Clear castling rights for the rook's side
        if (us == WHITE) {
            if (move.from == sq_index('h', '1')) {
                w_can_castle_k = false; // Kingside rook moved
            } else if (move.from == sq_index('a', '1')) {
                w_can_castle_q = false; // Queenside rook moved
            }
        } else {
            if (move.from == sq_index('h', '8')) {
                b_can_castle_k = false; // Kingside rook moved
            } else if (move.from == sq_index('a', '8')) {
                b_can_castle_q = false; // Queenside rook moved
            }
        }
    }

    if (move.captured == ROOK && !move.is_en_passant) {
        if (them == WHITE) {
            if (move.to == sq_index('h', '1')) {
                w_can_castle_k = false; // Opponent's kingside rook captured
            } else if (move.to == sq_index('a', '1')) {
                w_can_castle_q = false; // Opponent's queenside rook captured
            }
        } else {
            if (move.to == sq_index('h', '8')) {
                b_can_castle_k = false; // Opponent's kingside rook captured
            } else if (move.to == sq_index('a', '8')) {
                b_can_castle_q = false; // Opponent's queenside rook captured
            }
        }
    }

    // Switch sides for turn to move 
    sideToMove = them; 

    // Recompute the occupancy bitboards
    recompute_occupancy();

    return undo; // Return the undo state (for legal move generation)
}

void Board::undo_move(const Move& move, const Undo& undo) {
    // Flip side to move back 
    sideToMove = (sideToMove == WHITE) ? BLACK : WHITE;

    // Reset pre-move state
    enPassantSquare = undo.ep_square;
    w_can_castle_k = undo.w_can_castle_k;
    w_can_castle_q = undo.w_can_castle_q;
    b_can_castle_k = undo.b_can_castle_k;
    b_can_castle_q = undo.b_can_castle_q;

    Color us = sideToMove; 
    Color them = (us == WHITE) ? BLACK : WHITE;
    
    // Remove the piece from the destination square
    int to = move.to; // Destination square
    clear_bit(bitboards[board_index(us, move.piece)], to); // Clear the piece from the destination square

    /*
    Special Cases:
    - En Passant Capture
    - Castling
    */

    // Undo Castling Rook Move (if needed)
    if (move.is_castle) {
        if (us == WHITE) {
            if (move.to == sq_index('g', '1')) {
                // Kingside: Rook moves f1->h1
                clear_bit(bitboards[board_index(WHITE, ROOK)], sq_index('f', '1'));
                set_bit(bitboards[board_index(WHITE, ROOK)], sq_index('h', '1')); // Place rook back on h1
            } else {
                // Queenside: Rook moves d1->a1
                clear_bit(bitboards[board_index(WHITE, ROOK)], sq_index('d', '1'));
                set_bit(bitboards[board_index(WHITE, ROOK)], sq_index('a', '1')); // Place rook back on a1
            }
        } else {
            if (move.to == sq_index('g', '8')) {
                // Kingside: Rook moves f8->h8
                clear_bit(bitboards[board_index(BLACK, ROOK)], sq_index('f', '8'));
                set_bit(bitboards[board_index(BLACK, ROOK)], sq_index('h', '8')); // Place rook back on h8
            } else {
                // Queenside: Rook moves d8->a8
                clear_bit(bitboards[board_index(BLACK, ROOK)], sq_index('d', '8'));
                set_bit(bitboards[board_index(BLACK, ROOK)], sq_index('a', '8')); // Place rook back on a8
            }
        }
    }

    // Restore moving piece back on from square
    int from_idx = board_index(us, move.piece); // Get the index for the piece type being moved
    set_bit(bitboards[from_idx], move.from); // Set the piece back on the source square

    // Restore any captured piece
    if (move.is_en_passant) {
        int capturedSquare = (us == WHITE ? move.to - 8 : move.to + 8); // Calculate the square of the captured pawn
        int capturedIndex = board_index(them, PAWN); // Get the index for the opponent's pawn
        set_bit(bitboards[capturedIndex], capturedSquare); // Restore the captured pawn on the board
    } else if (undo.captured != NO_PIECE) {
        int capturedIndex = board_index(them, undo.captured); // Get the index for the captured piece
        set_bit(bitboards[capturedIndex], move.to); // Restore the captured piece on the destination square
    }

    // Recompute the occupancy bitboards 
    recompute_occupancy();
}