#include "board.hpp"
#include <iostream> 

int main() {
    // Create a Board instance
    Board board; 

    // Initialize the board to the starting position
    board.init_startpos();

    // Recompute the occupancy bitboards
    board.recompute_occupancy();

    // Display the bitboards
    for (size_t i = 0; i < board.bitboards.size(); ++i) {
        std::cout << "Bitboard for piece type " << i << ":\n";
        std::cout << "-----------------\n";
        display_bitboard(board.bitboards[i]);
    }

    // Display the occupancy bitboards
    std::cout << "Both Occupancy:\n";
    display_bitboard(board.bothOccupancy);

    std::cout << "White Occupancy:\n";
    display_bitboard(board.whiteOccupancy);

    std::cout << "Black Occupancy:\n";
    display_bitboard(board.blackOccupancy);
    std::cout << "-----------------\n";

    std::cout << "Board initialized and occupancy recomputed successfully.\n";
    
    return 0;
}

