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
- `generate_pseudo_legal_moves`: Generate all the pseudo-legal moves from a particular game state.