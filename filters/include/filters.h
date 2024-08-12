#pragma once

#include <SDL2/SDL.h>
#include <math.h>

#define _USE_MATH_DEFINES

SDL_Surface *grayscale_filter(SDL_Surface *inputSurface);

SDL_Surface *threshold_filter(SDL_Surface *inputSurface);

SDL_Surface *invert_filter(SDL_Surface *inputSurface);

SDL_Surface *gaussian_filter(SDL_Surface *inputSurface);

SDL_Surface *sobel_filter(SDL_Surface *inputSurface);

SDL_Surface *hough_transform_filter(SDL_Surface *inputSurface);

SDL_Surface *rotate_filter(SDL_Surface *inputSurface, double angle);
void draw_sudoku_grid_outline(SDL_Surface *input);