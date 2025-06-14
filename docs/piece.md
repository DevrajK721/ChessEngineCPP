# Piece 
Documentation for `piece.hpp`. 

---

## Piece Index Scheme 
### Description 
The index scheme defined must be consistent throughout the engine for use when determining which pieces are on what bitboards. 

### Setup
- `Color`
    1. `WHITE = 1`
    2. `BLACK = -1`
    3. `BOTH = 0`
- `PieceType`
    1. `PAWN = 1 `
    2. `KNIGHT = 2`
    3. `BISHOP = 3`
    4. `ROOK = 4`
    5. `QUEEN = 5`
    6. `KING = 6`
    7. `NO_PIECE = 0`

### Functions 
- `piece_index`: Applies simple operation to piece definition to give it a number
    - A Black Knight would be $-1 \times 2 = -2$
    - A White King would be $1 \times 6 = 6$
- `board_index`: Applies simple transformation for storing 12 bitboards in a simple array
    - Index 0-5 = White Pawn->King
    - Index 6-11 = Black Pawn->King
    - `(c == WHITE ? (pt - 1) : ((pt - 1) + 6))` subtracts 1 and makes it negative for black pieces but keeps it the same sign for white pieces. 