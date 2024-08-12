#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int load_grid(int grid[9][9], const char *filename);

int find_empty_cell(const int grid[9][9], int *row, int *col);

int is_number_placement_valid(const int grid[9][9]
		     ,int row,int col,int number);

int solve_grid(int grid[9][9]);

int save_grid(const int grid[9][9], const char *filename);
 
 
