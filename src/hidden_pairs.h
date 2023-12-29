#pragma once


#include "sudoku.h"

typedef struct {
    int value1;
    int value2;
    Cell *p_cell;
} HiddenPair;

bool is_hidden_pair(Cell **p_cells, int value1, int value2);
bool is_in_cell(Cell *p_cell, int value1, int value2);
int find_hidden_double_values(Cell **p_cells, int *hidden_double_values);
void find_hidden_double(Cell **p_cells, HiddenPair *p_hidden_double,
                        int *p_counter);
int hidden_pairs(SudokuBoard *p_board);
