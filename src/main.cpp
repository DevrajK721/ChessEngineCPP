#include "movegen.hpp"
#include <iostream> 

int main() {
    init_leaper_attacks();
    init_pawn_attacks();

    // Create a Board instance
    Board board; 

    // Initialize the board to the starting position
    board.init_startpos();

    // Compute the occupancy bitboards
    board.recompute_occupancy();

    // Generate legal moves for white (20 moves)
    MoveList whiteMoves = generate_legal_moves(board, WHITE);
    std::cout << "White Pseudo-Legal Moves: " << whiteMoves.size() << std::endl;
    for (const auto &move : whiteMoves) {
        std::cout << "From: " << move.from << ", To: " << move.to 
                  << ", Piece: " << move.piece 
                  << ", Captured: " << move.captured 
                  << ", Double Push: " << move.is_double_push << std::endl;
    }
    
    // Generate legal moves for knight in middle of the board (8 moves)
    Board board2; 
    for (auto &bb : board2.bitboards) {
        bb = 0ULL; // Clear all bitboards
    }

    board2.recompute_occupancy();

    int sq = sq_index('d', '4');
    set_bit(board2.bitboards[board_index(WHITE, KNIGHT)], sq); // Place a knight on d4
    board2.recompute_occupancy();

    MoveList knightMoves = generate_legal_moves(board2, WHITE);
    std::cout << "Knight Pseudo-Legal Moves: " << knightMoves.size() << std::endl;
    for (const auto &move : knightMoves) {
        std::cout << "From: " << move.from << ", To: " << move.to
                  << ", Piece: " << move.piece
                  << ", Captured: " << move.captured
                  << ", Double Push: " << move.is_double_push << std::endl;
    }

    return 0; 

}

