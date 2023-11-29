#include <assert.h>
<<<<<<< HEAD
=======
#include <curses.h>
>>>>>>> 34064c6 (Initial commit)
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

<<<<<<< HEAD
#include "../src/sudoku.h"
#include "../src/hidden_singles.h"
#include "../src/hidden_pairs.h"
#include "../src/hidden_triples.h"
#include "../src/naked_pairs.h"
#include "../src/naked_triples.h"
=======
#include "../src/common.h"
#include "../src/game.h"
#include "../src/game_setup.h"
#include "../src/mbstrings.h"
>>>>>>> 34064c6 (Initial commit)

// Verbosity of test runner. Overridden via compilation flag
#ifdef VERBOSE
#define VERBOSE 1
#endif
#ifndef VERBOSE
#define VERBOSE 0
#endif

<<<<<<< HEAD
char mapping[32] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};

int get_index(char *array, int len, char input)
{
    for (int i = 0; i < len; i++)
    {
        if (array[i] == input)
            return i;
    }
    return -1;
}

void toBinary(int n, int *bin_array)
{
    int id = 0;
    while (n)
    {
        bin_array[id++] = n % 2;
        n = n / 2;
    }
}

int toInteger(int *bin_array, int len)
{
    int num = 0;
    for (int i = 0; i < len; i++)
    {
        num += bin_array[i] * (1 << i);
    }
    return num;
}

void load_cell_candidates(Cell *p_cell, char *textData)
{
    int left_num = get_index(mapping, 32, textData[0]);
    int right_num = get_index(mapping, 32, textData[1]);

    int bin_candidates[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    toBinary(right_num, &(bin_candidates[0]));
    toBinary(left_num, &(bin_candidates[5]));

    int counter = 0;
    int candidates[BOARD_SIZE];

    for (int cand = 1; cand <= 9; cand++)
    {
        if (bin_candidates[cand])
            candidates[counter++] = cand;
    }

    set_candidates(p_cell, candidates, counter);
    if (bin_candidates[0])
        p_cell->fixed = true;
    else
        p_cell->fixed = false;
}

void load_sudoku_with_candidates(SudokuBoard *p_board, char *textData)
{
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        load_cell_candidates(&(p_board->data[i / BOARD_SIZE][i % BOARD_SIZE]), textData);
        textData += 2;
    }
}

void print_string_candidates(Cell *p_cell, char *textData)
{
    int *candidates = get_candidates(p_cell);
    int len = p_cell->num_candidates;

    int bin_candidates[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (p_cell->fixed)
        bin_candidates[0] = 1;

    for (int i = 0; i < len; i++)
    {
        bin_candidates[candidates[i]] = 1;
    }
    int left_index = toInteger(&(bin_candidates[5]), 5);
    int right_index = toInteger(&(bin_candidates[0]), 5);
    sprintf(textData, "%c%c", mapping[left_index], mapping[right_index]);
    free(candidates);
}

void print_sudoku_with_candidates(SudokuBoard *p_board, char *textData)
{
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        print_string_candidates(&(p_board->data[i / BOARD_SIZE][i % BOARD_SIZE]), textData);
        textData += 2;
    }
}

typedef int (*method)(SudokuBoard *p_board);

method get_method(char *method_name)
{
    if (strcmp(method_name, "hidden_singles") == 0)
        return hidden_singles;
    else if (strcmp(method_name, "naked_pairs") == 0)
        return naked_pairs;
    else if (strcmp(method_name, "hidden_pairs") == 0)
        return hidden_pairs;
    else if (strcmp(method_name, "naked_triples") == 0)
        return naked_triples;
    else if (strcmp(method_name, "hidden_triples") == 0)
        return hidden_triples;
    else
        return NULL;
=======
enum input_key get_input(char c)
{
    if (c == 'U')
    {
        return INPUT_UP;
    }
    else if (c == 'D')
    {
        return INPUT_DOWN;
    }
    else if (c == 'L')
    {
        return INPUT_LEFT;
    }
    else if (c == 'R')
    {
        return INPUT_RIGHT;
    }
    else if (c == 'N')
    {
        return INPUT_NONE;
    }
    else
    {
        fprintf(stderr, "Invalid input character %c\n", c);
        exit(EXIT_FAILURE);
    }
}

void print_game(int *cells, size_t height, size_t width)
{
    setlocale(LC_CTYPE, "");
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            char cell = cells[i * width + j];
            if (cell == FLAG_PLAIN_CELL)
            {
                printf(".");
            }
            else if (cell == FLAG_SNAKE)
            {
                printf("S");
            }
            else if (cell == FLAG_WALL)
            {
                printf("%lc", (wint_t)0x02588);
            }
            else if (cell == FLAG_FOOD)
            {
                printf("O");
            }
            else
            {
                printf("?");
            }
        }
        printf("\n");
    }
    printf("\n");
}

// returns 0 if success, or a board decompress error code if failure
int run_test(int **cells_p, size_t *width_p, size_t *height_p, snake_t *snake_p,
             char *board_rep, unsigned int snake_grows, char *input_string)
{
    int status = initialize_game(cells_p, width_p, height_p, snake_p, board_rep);

    // return early if error parsing board
    if (status != INIT_SUCCESS)
    {
        return status;
    }

    int i = 0;
    while (1)
    {
        if (VERBOSE)
        {
            printf("Board at time step %d:\n", i);
            print_game(*cells_p, *height_p, *width_p);
        }
        // if we reach the end of the input, the trace is over
        if (*input_string == '\0')
        {
            break;
        }

        // Get user input
        enum input_key input = get_input(*input_string);
        input_string += 1;

        // Update game state
        update(*cells_p, *width_p, *height_p, snake_p, input, snake_grows);

        i += 1;
    }

    return 0;
>>>>>>> 34064c6 (Initial commit)
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
    if (argc < 4)
    {
        printf("Usage: %s <sudoku string> <method> <pipe>\n\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    SudokuBoard *board = malloc(sizeof(SudokuBoard));
    init_sudoku(board);
    load_sudoku_with_candidates(board, argv[1]);

    FILE *pipe = fdopen(atoi(argv[3]), "w");

    int num_detected = get_method(argv[2])(board);

    char *outText = malloc(BOARD_SIZE * BOARD_SIZE * 2 + 1);
    outText[0] = '\0';

    print_sudoku_with_candidates(board, outText);

    fprintf(pipe,
            "{\n"
            "    \"num_detects\": %d,\n"
            "    \"boards\": \"%s\"\n"
            "}\n",
            num_detected, outText);

    free_sudoku(board);
    free(board);
    free(outText);

=======
    if (getenv("DEBUG"))
    {
        sleep(1);
    }

    if (argc < 6)
    {
        printf(
            "Usage: autograder <board_string> <seed> <snake_grows> <key_input>"
            " <consider_name> <pipe>\n");
        exit(EXIT_FAILURE);
    }

    char *board_string = argv[1];
    unsigned int seed = atoi(argv[2]);
    unsigned int snake_grows = atoi(argv[3]); // Should be 0 or 1
    char *key_input = argv[4];
    unsigned int consider_name = atoi(argv[5]); // Should be 0 or 1
    FILE *pipe = fdopen(atoi(argv[6]), "w");

    set_seed(seed);
    // if no board string is provided then use the default board by setting
    // null
    if (board_string[0] == '0')
    {
        board_string = NULL;
    }

    // Run the snake game
    int *cells;
    snake_t snake;
    // default the board to 0x0 so the stencil doesn't crash
    size_t width = 0;
    size_t height = 0;

    int status =
        run_test(&cells, &width, &height, &snake, board_string, snake_grows, key_input);

    if (status != INIT_SUCCESS)
    {
        char *msg = "";
        if (status == INIT_ERR_BAD_CHAR)
        {
            msg = "BAD_CHAR";
        }
        else if (status == INIT_ERR_INCORRECT_DIMENSIONS)
        {
            msg = "INCORRECT_DIMENSIONS";
        }
        else if (status == INIT_ERR_WRONG_SNAKE_NUM)
        {
            msg = "WRONG_SNAKE_NUM";
        }
        fprintf(pipe,
                "{\n"
                "    \"board_error\": \"%s\"\n"
                "}\n",
                msg);
        teardown(cells, &snake);
        exit(EXIT_SUCCESS);
    }

    char *cell_string = (char *)malloc(
        width * height + 1);
    if (cell_string == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for cell string\n");
        teardown(cells, &snake);
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            char cell = cells[i * width + j];
            char cell_as_char;
            if (cell == FLAG_PLAIN_CELL)
            {
                cell_as_char = '.';
            }
            else if (cell == FLAG_SNAKE)
            {
                cell_as_char = 'S';
            }
            else if (cell == FLAG_WALL)
            {
                cell_as_char = 'X';
            }
            else if (cell == FLAG_FOOD)
            {
                cell_as_char = 'O';
            }
            else
            {
                cell_as_char = '?';
            }
            cell_string[i * width + j] = cell_as_char;
        }
    }
    cell_string[width * height] = '\0';

    if (consider_name)
    {
        // Test name reading, mbslen
        char name_buf[1000];
        read_name(name_buf);
        size_t name_len = mbslen(name_buf);

        // Write bytes of name_buf into string
        char name_byte_str_buf[2000];
        assert(strlen(name_buf) < 1000);
        for (size_t i = 0; i <= strlen(name_buf); ++i)
        {
            if (name_buf[i] == 0)
            {
                break;
            }
            // Write two hex digits at a time, offset by (i * 2) —
            // each written string is three bytes long, so we overwrite the
            // trailing (null) byte with each write, giving us a continuous
            // string of hex values.
            snprintf(name_byte_str_buf + (i * 2), 3, "%02x",
                     (unsigned char)name_buf[i]);
        }

        fprintf(pipe,
                "{\n"
                "    \"game_over\": %d,\n"
                "    \"score\": %d,\n"
                "    \"name\": \"%s\",\n"
                "    \"name_len\": %lu,\n"
                "    \"width\": %lu,\n"
                "    \"height\": %lu,\n"
                "    \"cells\": \"%s\"\n"
                "}\n",
                g_game_over, g_score,
                name_byte_str_buf, name_len, width,
                height, cell_string);
    }
    else
    {
        fprintf(pipe,
                "{\n"
                "    \"game_over\": %d,\n"
                "    \"score\": %d,\n"
                "    \"width\": %lu,\n"
                "    \"height\": %lu,\n"
                "    \"cells\": \"%s\"\n"
                "}\n",
                g_game_over, g_score,
                width, height,
                cell_string);
    }

    teardown(cells, &snake);
    free(cell_string);
>>>>>>> 34064c6 (Initial commit)
    fclose(pipe);
    exit(EXIT_SUCCESS);
}