#include "solver.h"

int load_grid(int grid[9][9], const char *filename)
{
    // Open file.
    FILE *stream = fopen(filename, "r");
    if (!stream)
    {
        return EXIT_FAILURE;
    }

    // Read the file.
    size_t row = 0, col;
    char c;
    while (row < 9)
    {
        col = 0;
        while (col < 9)
        {
            c = fgetc(stream);
            if (c >= '0' && c <= '9')
            {
                grid[row][col] = c - '0';
                col++;
            }
            else if (c == '.')
            {
                grid[row][col] = 0;
                col++;
            }
        }

        row++;
    }

    // Close file.
    fclose(stream);

    return EXIT_SUCCESS;
}

int find_empty_cell(const int grid[9][9], int *row, int *col)
{
    // For each cell, check if it is equal to 0.
    *row = 0;
    while (*row < 9)
    {
        *col = 0;
        while (*col < 9)
        {
            if (grid[*row][*col] == 0)
            {
                return EXIT_SUCCESS;
            }

            (*col)++;
        }

        (*row)++;
    }

    return EXIT_FAILURE;
}

int is_number_placement_valid(const int grid[9][9], int row, int col,
                              int number)
{
    // Check row and col.
    size_t i = 0;
    while (i < 9)
    {
        if (grid[i][col] == number || grid[row][i] == number)
        {
            return EXIT_FAILURE;
        }

        i++;
    }

    // Check square.
    size_t squareX = row / 3;
    size_t squareY = col / 3;
    for (size_t x = 0; x < 3; x++)
    {
        for (size_t y = 0; y < 3; y++)
        {
            if (grid[squareX * 3 + x][squareY * 3 + y] == number)
            {
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}

int solve_grid(int grid[9][9])
{
    // Find the first empty cell.
    int row, col;
    if (find_empty_cell(grid, &row, &col))
    {
        return EXIT_SUCCESS;
    }

    // Test all numbers.
    for (int n = 1; n < 10; n++)
    {
        // If number is correct, place it and call on the next empty cell.
        if (!is_number_placement_valid(grid, row, col, n))
        {
            grid[row][col] = n;
            if (!solve_grid(grid))
            {
                return EXIT_SUCCESS;
            }

            // If it does not work, go back to empty cell.
            grid[row][col] = 0;
        }
    }

    // No solution found.
    return EXIT_FAILURE;
}

int save_grid(const int grid[9][9], const char *filename)
{
    // Name.
    char *suffix = ".result";
    char *fullname = malloc(strlen(filename) + strlen(suffix) + 1);
    strcpy(fullname, filename);
    strcat(fullname, suffix);

    // Open file and free fullname.
    FILE *stream = fopen(fullname, "w");
    free(fullname);
    if (!stream)
    {
        return EXIT_FAILURE;
    }

    // Write in file.
    size_t row = 0, col;
    while (row < 9)
    {
        col = 0;
        while (col < 9)
        {
            fputc(grid[row][col] + '0', stream);
            col++;
            if (col % 3 == 0 && col != 9)
            {
                fputc(' ', stream);
            }
        }

        row++;
        if (row != 9)
        {
            fputc('\n', stream);
            if (row % 3 == 0)
            {
                fputc('\n', stream);
            }
        }
    }

    fputc('\n', stream);

    // Close file.
    fclose(stream);

    return EXIT_SUCCESS;
}
