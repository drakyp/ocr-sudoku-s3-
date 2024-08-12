#include <stdlib.h>
#include <err.h>
#include "solver.h"

int main(int argc, char *argv[])
{
    // Check that argc is 2.
    if (argc != 2)
    {
        errx(EXIT_FAILURE, "Usage: solver <sudoku grid>");
    }

    // Grid variable.
    int grid[9][9];

    // Load grid file into grid.
    if (load_grid(grid, argv[1]))
        errx(EXIT_FAILURE, "Error: couldn't open grid file '%s'.", argv[1]);

    // Solve grid if possible.
    if (solve_grid(grid))
        errx(EXIT_FAILURE, "Error: the grid cannot be solved.");

    // Save the grid.
    if (save_grid(grid, argv[1]))
        errx(EXIT_FAILURE, "Error: couldn't save grid file.");

    // Return success.
    return EXIT_SUCCESS;
}
