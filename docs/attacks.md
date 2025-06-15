# Attacks 
Documentation for `attacks.hpp` & `attacks.cpp`. 

---

## Leaper Attacks
### Description 
Knights and Kings are considered *leapers* as their attacks do not depend on other pieces. We can therefore precompute for the each square the `uint64_t` mask of all the squares they could jump to. 

### Functions
- `init_leaper_attacks()`: Compute all possible moves from each square for king and knights. 


## Pawn Attacks 
### Description 
Pawn attacks have to be handled separately for white and black. White attacks North-East and North-West directions while Black attacks South-East and South-West directions. 

### Functions 
- `init_pawn_attacks()`: Compute all possible moves from each square for white and black pawns

## Sliding Piece Attacks
### Description 
The sliding pieces depend on blockers and we can use directional vectors for the piecers:
- Rook: N, E, S, W
- Bishop: NE, SE, SW, NW
- Queen: Rook & Bishop

### Functions 
- `sliding_attacks()`: Compute the sliding attacks for a specific piece out of the rook, bishop or queen from a certain position using occupancy bitboards.
    - `const int directions[]`
        - An array of “direction offsets” (e.g. +1, –1, +8, –8 for rooks; ±7, ±9 for bishops) that tell you which way to step from the square 
	- int dirCount
        - The number of entries in the directions array.
- `rook_attacks`: Compute sliding attacks for rook 
- `bishop_attacks`: Compute sliding attacks for bishop 
- `queen_attacks`: Compute sliding attacks for queen 