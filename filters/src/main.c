#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include "filters.h"

int main(int argc, char *argv[])
{
    // Check arguments.
    if (argc < 3)
    {
        errx(EXIT_FAILURE, "Usage: filters <image> <filter>");
    }

    // Load image.
    SDL_Surface *inputSurface = IMG_Load(argv[1]);
    if (!inputSurface)
    {
        errx(EXIT_FAILURE, "Error: couldn't load input image.");
    }
    inputSurface = SDL_ConvertSurfaceFormat(inputSurface,
                                            SDL_PIXELFORMAT_RGB888, 0);
    if (!inputSurface)
    {
        errx(EXIT_FAILURE, "Error: couldn't convert input image.");
    }

    // Select filter.
    SDL_Surface *outputSurface;

    if (!strcmp(argv[2], "grayscale"))
    {
        outputSurface = grayscale_filter(inputSurface);
    }
    else if (!strcmp(argv[2], "threshold"))
    {
        if (argc != 4)
        {
            errx(EXIT_FAILURE, "Usage: filters <image> threshold <threshold>");
        }

        outputSurface = threshold_filter(inputSurface, atoi(argv[3]));
    }
    else if (!strcmp(argv[2], "rotate"))
    {
        if (argc != 4)
        {
            errx(EXIT_FAILURE, "Usage: filters <image> rotate <angle>");
        }

        outputSurface = rotate_filter(inputSurface, atof(argv[3]));
    }
    else if (!strcmp(argv[2], "invert"))
    {
        outputSurface = invert_filter(inputSurface);
    }
    else if (!strcmp(argv[2], "sobel"))
    {
        outputSurface = sobel_filter(inputSurface);
    }
    else if (!strcmp(argv[2], "hough"))
    {
        if (argc != 4)
        {
            errx(EXIT_FAILURE, "Usage: filters <image> hough <threshold>");
        }

        outputSurface = hough_transform(inputSurface, atof(argv[3]));
    }
    else if (!strcmp(argv[2], "gaussian"))
    {
        outputSurface = gaussian_filter(inputSurface);
    }
    else if (!strcmp(argv[2], "all"))
    {
        outputSurface = grayscale_filter(inputSurface);
        outputSurface = gaussian_filter(outputSurface);
        if (argc >= 4)
        {
            outputSurface = threshold_filter(outputSurface, atoi(argv[3]));
        }
        else
        {
            outputSurface = threshold_filter(outputSurface, 128);
        }

        outputSurface = invert_filter(outputSurface);
        outputSurface = sobel_filter(outputSurface);

        if (argc == 5)
        {
            outputSurface = hough_transform(outputSurface, atoi(argv[4]));
        }
        else
        {
            outputSurface = hough_transform(outputSurface, 500);
        }
    }
    else
    {
        errx(EXIT_FAILURE, "Error: unknown filter '%s'", argv[2]);
    }

    // Save output image.
    IMG_SavePNG(outputSurface, "output.png");

    return EXIT_SUCCESS;
}
