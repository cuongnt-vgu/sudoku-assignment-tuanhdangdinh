
#pragma once
 
#include "sudoku.h"
 
struct naked_pair_reserved {
    struct naked_pair_reserved *next;
    int row;
    int col;
    int box;
    int candidate1;
    int candidate2;
};
int naked_pairs(SudokuBoard *board);
bool naked_pairable(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate1, int idCandidate2);
void insert_naked_pair(struct naked_pair_reserved **HEAD, int row, int col,int box, int candidate1, int candidate2);
void transverse_naked_pair(struct naked_pair_reserved *HEAD, SudokuBoard *board);
void remove_candidate_naked_pair(struct naked_pair_reserved *HEAD, SudokuBoard *board);
void free_naked_pair (struct naked_pair_reserved *HEAD);
 