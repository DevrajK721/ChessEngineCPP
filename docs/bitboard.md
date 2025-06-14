# Bitboard 
Documentation for `Bitboard.hpp`. 

---

## Basic Bitboard Operations 
### Description
A bitboard is simply a 64-bit integer (`uint64_t`) where each bit corresponds to one square on a chess board. 
- The LSB (Least Significant Bit) is A1 (bit 0)
- The MSB (Most Significant Bit) is H8 (bit 63)

### Functions
- `set_bit`: Sets the bit (turns to 1) at position "sq" in the bitboard "b".
    1. `(1ULL << sq)` creates a 64-bit mask with a 1 in the "sq" position and 0s elsewhere.
    2. `b |= mask` performs a bitwise OR (sets to 1 if either bit is 1) to set the bit at "sq" in "b" to 1.

- `clear_bit`: Clears the bit (turns to 0) at position "sq" in the bitboard "b".
    1. `(1ULL << sq)` as before creates a mask with a 1 in the "sq" position.
    2. `~mask` inverts the bits so the mask has 0 at "sq" and 1s elsewhere now. 
    3. `b &= mask` performs a bitwise AND (sets to 0 if either bit is 0) to clear the bit at "sq" in "b". 

- `test_bit`: Returns true if the bit at position "sq" in the bitboard "b" is set to 1, false otherwise.
    1. `(b >> sq)` shifts the bits of "b" right by "sq" positions so the bit at "sq" is now in the least significant position (bit 0).
    2. `& 1` checks if the LSB (least significant bit) is 1, which indicates the bit at "sq" was set.

- `pop_lsb`: Clears and returns the index of the least significant bit that is set to 1 in the bitboard "b".
    1. `__builtin_ctzll(b)` counts the number of trailing zeros in "b", e.g. 0000000100 has 2 trailing zeros, so it returns 2. 
    2. `b &= (b - 1)` clears the least significant bit that is set to 1, effectively removing it from "b" (e.g. 0000000100 becomes 0000000000).
    3. This function is useful for iterating through all set bits in a bitboard, as it allows you to process each set bit one by one.

## Mapping File and Rank to Square Index 
### Description
We want to have a simple way of converting human-readable squares (e.g. e4) to the bit index (0 - 63). 
- File: 0=A, 1=B, 2=C, ..., 7=H
- Rank: 0=1, 1=2, 2=3, ..., 7=8

### Functions
- `sq_index` (1): Used for computing the square index (0-63) from the file and rank. 
    1. `rank * 8 + file` returns the square index 
- `sq_index` (2): Used for computing the square index from ASCII Character Input
    1. For example, consider square e4, `file = 4`, `rank = 3`, we get square index of 28. 

## Display Bitboard 
### Description 
Visualizing the chess board allows for debugging. 
