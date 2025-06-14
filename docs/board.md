# Piece 
Documentation for `board.cpp` & `board.hpp`. 

---

## Board Struct 
### Description
We must now define the 12 bitboards for all the pieces as well as initialize the starting position and update the occupancy bitboards. 

### Functions 
- `set_bits`: Helper function to set several bits at once on a bitboard
- `init_startpos`: Initialize the bitboards for the start of a chess game
- `recompute_occupancy`: Update the occupied squares (white, black and both)

