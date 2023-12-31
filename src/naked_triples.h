#pragma once

#include "sudoku.h"
typedef struct NakedTripleimpl{
    Cell * p_cell1;
    Cell* p_cell2;
    Cell* p_cell3;
    int values[3];
} NakedTriple;
int find_naked_triple_values(Cell **p_cells, int *naked_triple_values, int **combinations, int combinationsCount);
void find_naked_triple(Cell **p_cells, NakedTriple *naked_triples, int *p_counter, int **combinations, int combinationsCount);
int naked_triples(SudokuBoard *p_board);
int factorial(int n) ;
void generateCombinationsRecursive(int *currentCombo, int currentIndex, int remaining, int **combinations, int *combinationsCount);
void generateCombinations(int size, int ***combinations, int *combinationsCount);
void generateCombinationsRecursive(int *currentCombo, int currentIndex, int remaining, int **combinations, int *combinationsCount);
void freeCombinations(int ***combinations, int combinationsCount) ;
bool cell_same_unit(Cell *cell1, Cell *cell2);