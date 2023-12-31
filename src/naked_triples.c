#include "naked_triples.h"
#include <stdlib.h>
#include <stdio.h>
int find_naked_triple_values(Cell **p_cells, int *naked_triple_values, int **combinations, int combinationsCount)
{
    int counter = 0;

    for (int combo = 0; combo < combinationsCount; combo++)
    {
        for (int i = 0; i < BOARD_SIZE - 2; i++)
        {
            if (p_cells[i]->num_candidates > 1 && p_cells[i]->num_candidates < 4)
            {
                for (int j = i + 1; j < BOARD_SIZE - 1; j++)
                {
                    if (p_cells[j]->num_candidates > 1 && p_cells[j]->num_candidates < 4)
                    {
                        for (int k = j + 1; k < BOARD_SIZE; k++)
                        {
                            if (p_cells[k]->num_candidates > 1 && p_cells[k]->num_candidates < 4) //&& !p_cells[i]->processed && !p_cells[j]->processed && !p_cells[k]->processed)
                            {
                                int match1 = 0, match2 = 0, match3 = 0;

                                // Check candidates for each cell in the combination
                                match1 += is_candidate(p_cells[i], combinations[combo][0]);
                                match1 += is_candidate(p_cells[i], combinations[combo][1]);
                                match1 += is_candidate(p_cells[i], combinations[combo][2]);

                                match2 += is_candidate(p_cells[j], combinations[combo][0]);
                                match2 += is_candidate(p_cells[j], combinations[combo][1]);
                                match2 += is_candidate(p_cells[j], combinations[combo][2]);

                                match3 += is_candidate(p_cells[k], combinations[combo][0]);
                                match3 += is_candidate(p_cells[k], combinations[combo][1]);
                                match3 += is_candidate(p_cells[k], combinations[combo][2]);


                                // Check if each candidate appears at least twice
                                if ((match1 + match2 + match3) >= (p_cells[i]->num_candidates + p_cells[j]->num_candidates + p_cells[k]->num_candidates))
                                {
                                    naked_triple_values[counter++] = combinations[combo][0];
                                    naked_triple_values[counter++] = combinations[combo][1];
                                    naked_triple_values[counter++] = combinations[combo][2];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return counter;
}

void find_naked_triple(Cell **p_cells, NakedTriple *naked_triples, int *p_counter, int **combinations, int combinationsCount)
{
    int naked_triple_values[BOARD_SIZE * BOARD_SIZE];
    int naked_triple_count = find_naked_triple_values(p_cells, naked_triple_values, combinations, combinationsCount);
    for (int i = 0; i < naked_triple_count; i += 3)
    {
        int value1 = naked_triple_values[i];
        int value2 = naked_triple_values[i + 1];
        int value3 = naked_triple_values[i + 2];
        // Find naked triple cells
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (p_cells[j]->num_candidates > 1 && p_cells[j]->num_candidates < 4)
            {
                for (int k = j + 1; k < BOARD_SIZE; k++)
                {
                    if (p_cells[k]->num_candidates > 1 && p_cells[k]->num_candidates < 4)
                    {
                        for (int l = k + 1; l < BOARD_SIZE; l++)
                        {
                            if (p_cells[l]->num_candidates > 1 && p_cells[l]->num_candidates < 4 && !p_cells[j]->processed && !p_cells[k]->processed && !p_cells[l]->processed)
                            {
                                // match if 3 cells has 2/3 candidates that match with value1, value2 or value3
                                int c, len, match;
                                bool flag1 = false;
                                bool flag2 = false;
                                bool flag3 = false;
                                for (c = 0, len = p_cells[j]->num_candidates, match = 0; c < len; c++)
                                {
                                    if (is_candidate(p_cells[j], value1) || is_candidate(p_cells[j], value2) || is_candidate(p_cells[j], value3))
                                    {
                                        match++;
                                    }
                                }
                                if (match >= 2)
                                {
                                    flag1 = true;
                                }
                                else
                                {
                                    flag1 = false;
                                }
                                for (c = 0, len = p_cells[k]->num_candidates, match = 0; c < len; c++)
                                {
                                    if (is_candidate(p_cells[k], value1) || is_candidate(p_cells[k], value2) || is_candidate(p_cells[k], value3))
                                    {
                                        match++;
                                    }
                                }
                                if (match >= 2)
                                {
                                    flag2 = true;
                                }
                                else
                                {
                                    flag2 = false;
                                }
                                for (c = 0, len = p_cells[k]->num_candidates, match = 0; c < len; c++)
                                {
                                    if (is_candidate(p_cells[l], value1) || is_candidate(p_cells[l], value2) || is_candidate(p_cells[l], value3))
                                    {
                                        match++;
                                    }
                                }
                                if (match >= 2)
                                {
                                    flag3 = true;
                                }
                                else
                                {
                                    flag3 = false;
                                }
                                if (flag1 == true && flag2 == true && flag3 == true)
                                {
                                    p_cells[j]->processed = true;
                                    p_cells[k]->processed = true;
                                    p_cells[l]->processed = true;
                                    naked_triples[*p_counter].p_cell1 = p_cells[j];
                                    naked_triples[*p_counter].p_cell2 = p_cells[k];
                                    naked_triples[*p_counter].p_cell3 = p_cells[l];
                                    naked_triples[*p_counter].values[0] = value1;
                                    naked_triples[*p_counter].values[1] = value2;
                                    naked_triples[*p_counter].values[2] = value3;
                                    (*p_counter)++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // match for non naked triple cells then remove
    for (int i = 0; i < *p_counter; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (p_cells[j]->num_candidates > 1)
            {
                // Skip naked triple cells
                if (p_cells[j] != naked_triples[i].p_cell1 && p_cells[j] != naked_triples[i].p_cell2 && p_cells[j] != naked_triples[i].p_cell3)
                {
                    // match if they are in the same row, same column or same box
                    if (cell_same_unit(p_cells[j], naked_triples[i].p_cell1) &&
                        cell_same_unit(p_cells[j], naked_triples[i].p_cell2) && cell_same_unit(p_cells[j], naked_triples[i].p_cell3) && !p_cells[j]->processed)
                    {
                        // Unset candidates
                        if (is_candidate(p_cells[j], naked_triples[i].values[0]))
                        {
                            unset_candidate(p_cells[j], naked_triples[i].values[0]);
                            p_cells[j]->processed = true;
                        }
                        if (is_candidate(p_cells[j], naked_triples[i].values[1]))
                        {
                            unset_candidate(p_cells[j], naked_triples[i].values[1]);
                            p_cells[j]->processed = true;
                        }
                        if (is_candidate(p_cells[j], naked_triples[i].values[2]))
                        {
                            unset_candidate(p_cells[j], naked_triples[i].values[2]);
                            p_cells[j]->processed = true;
                        }
                    }
                }
            }
        }
    }
}
int naked_triples(SudokuBoard *p_board)
{
    NakedTriple naked_triples[BOARD_SIZE * BOARD_SIZE];
    int total_naked_triples = 0;
    int **combinations;
    int combinationsCount;
    generateCombinations(BOARD_SIZE, &combinations, &combinationsCount);

    // match rows for naked triples
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_naked_triple(p_board->p_rows[i], naked_triples, &total_naked_triples, combinations, combinationsCount);
    }

    // match columns for naked triples
    for (int j = 0; j < BOARD_SIZE; j++)
    {
        find_naked_triple(p_board->p_cols[j], naked_triples, &total_naked_triples, combinations, combinationsCount);
    }

    // match boxes for naked triples
    for (int k = 0; k < BOARD_SIZE; k++)
    {
        find_naked_triple(p_board->p_boxes[k], naked_triples, &total_naked_triples, combinations, combinationsCount);
    }

    // Free the allocated memory
    freeCombinations(&combinations, combinationsCount);

    return total_naked_triples;
}


bool cell_same_unit(Cell *cell1, Cell *cell2)
{
    return (cell1->row_index == cell2->row_index) || (cell1->col_index == cell2->col_index) || (cell1->box_index == cell2->box_index);
}
bool isElementPresent(int element, int *array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == element) {
            return true;
        }
    }
    return false;
}

void generateCombinations(int n, int ***combinations, int *combinationsCount)
{
    // Calculate the number of combinations
    *combinationsCount = n * (n - 1) * (n - 2) / 6;

    // Allocate memory for the array of combinations
   *combinations = (int **)malloc(*combinationsCount * sizeof(int *));
    for (int i = 0; i < *combinationsCount; ++i)
    {
        (*combinations)[i] = (int *)(malloc(3 * sizeof(int)));
    }

    // Generate and store combinations
    int index = 0;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = i + 1; j <= n; ++j)
        {
            for (int k = j + 1; k <= n; ++k)
            {
                // Store the combination in the array
                (*combinations)[index][0] = i;
                (*combinations)[index][1] = j;
                (*combinations)[index][2] = k;
                index++;
            }
        }
    }
}

void freeCombinations(int ***combinations, int combinationsCount)
{
    // Free memory for the array of combinations
    for (int i = 0; i < combinationsCount; ++i)
    {
        free((*combinations)[i]);
    }
    free(*combinations);
}