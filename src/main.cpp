#include "bitboard.hpp"
#include <iostream> 

int main() {
    U64 bb = 0; // Initialize a bitboard to zero
    // Pawn Opening 
    set_bit(bb, sq_index('e', '4')); // Set the bit for e4
    set_bit(bb, sq_index('e', '5')); // Set the bit for e5 

    std::cout << "Test Bitboard:\n";
    display_bitboard(bb); // Display the bitboard
    
    return 0;
}

