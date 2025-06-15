# Movegen 
Documentation for `movegen.hpp` and `movegen.cpp`

---

## Generating Moves 
### Description 
Move generation is key to ensure no illegal moves are played in the game. 

### Move Struct 
The move struct contains 5 vital pieces of information for defining a `Move` in this engine:
1. `from`: The piece we are moving from 
2. `to`: The piece we are moving to 
3. `piece`: What piece we are moving 
4. `captured`: NO_PIECE if quiet else it will show the captured piece type 
5. `is_double_push`: True for pawn two-step move 

### Functions
- `generate_pseudo_legal_moves`: Generate all the legal moves from a particular game state.
- `generate_legal_moves`: Remove the illegal moves from the pseudo-legal moves generation (i.e. ones that put the king in check) by making the move, checking if the king square then becomes under attack (the move gets removed if it does) then undoes the move. 
