# Board 
Documentation for `board.cpp` & `board.hpp`. 

---

## Board Struct 
### Description
We must now define the 12 bitboards for all the pieces as well as initialize the starting position and update the occupancy bitboards. 

### Functions 
- `set_bits`: Helper function to set several bits at once on a bitboard
- `init_startpos`: Initialize the bitboards for the start of a chess game
- `recompute_occupancy`: Update the occupied squares (white, black and both)

## Move Filtering and Attack Detection 
### Description
In order to continue to build the pseudo-legal move generator to handle checks and attacks, we must incorporate functions which check when squares are attacked (especially the king) and also have correct capture logic. 

### Functions
- `is_square_attacked`: Checks if a particular square is under threat by an opposing piece. 
- `make_move`: Make a move and update bitboards
- `undo_move`: Undo a move and update bitboards