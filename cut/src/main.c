#include <err.h>
#include "cut.h"

int main(int argc, char *argv[])
{
    // 3 arguments;
    if (argc != 3)
    {
        errx(EXIT_FAILURE, "Usage: cut <image> <number of cut>");
    }

    // Load image.
    SDL_Surface *image = IMG_Load(argv[1]);
    if (!image)
    {
        errx(EXIT_FAILURE, "Error: couldn't open image.");
    }

    // Cut image.
    cut_and_save(image, strtoul(argv[2], NULL, 10));

    return EXIT_SUCCESS;
}
