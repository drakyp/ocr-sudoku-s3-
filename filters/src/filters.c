#include <SDL2/SDL.h>
#include <err.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Takes a colored image.
SDL_Surface *grayscale_filter(SDL_Surface *inputSurface)
{
    // Lock input surface.
    if (SDL_LockSurface(inputSurface))
    {
        errx(EXIT_FAILURE,
             "Error: couldn't lock the surface (grayscale_filter).");
    }

    // Create output surface.
    SDL_Surface *outputSurface =
        SDL_CreateRGBSurfaceWithFormat(0, inputSurface->w, inputSurface->h,
                                       32, SDL_PIXELFORMAT_RGBA8888);
    if (!outputSurface)
    {
        errx(EXIT_FAILURE,
             "Error: couldn't create a new surface (grayscale_filter).");
    }

    // Lock output surface.
    if (SDL_LockSurface(outputSurface))
    {
        errx(EXIT_FAILURE,
             "Error: couldn't lock the surface (grayscale_filter).");
    }

    // Process each pixels.
    SDL_Color c;
    Uint8 average;
    SDL_PixelFormat *inputFormat = inputSurface->format;
    SDL_PixelFormat *outputFormat = outputSurface->format;
    Uint32 *inputPixels = inputSurface->pixels;
    Uint32 *outputPixels = outputSurface->pixels;
    for (int i = 0; i < inputSurface->w * inputSurface->h; i++)
    {
        // Get pixel color.
        SDL_GetRGB(inputPixels[i], inputFormat, &c.r, &c.g, &c.b);

        // Compute average.
        average = (c.r + c.g + c.b) / 3;

        // Set output pixel.
        outputPixels[i] = SDL_MapRGB(outputFormat, average, average, average);
    }

    // Unlock surfaces.
    SDL_UnlockSurface(inputSurface);
    SDL_UnlockSurface(outputSurface);

    return outputSurface;
}

// Takes a grayscale image.
SDL_Surface *threshold_filter(SDL_Surface *input)
{
    int threshold = 200;

    // Try to lock input surface.
    if (SDL_LockSurface(input))
    {
        errx(EXIT_FAILURE, "Error: %s (threshold_filter)", SDL_GetError());
    }

    // Try to create and lock output surface.
    SDL_Surface *output = SDL_CreateRGBSurfaceWithFormat(0, input->w, input->h, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!output)
    {
        errx(EXIT_FAILURE, "Error: %s (threshold_filter)", SDL_GetError());
    }

    // Apply filter.
    Uint32 *input_pixels = input->pixels;
    Uint32 *output_pixels = output->pixels;
    Uint32 white = SDL_MapRGB(output->format, 255, 255, 255);
    Uint32 black = SDL_MapRGB(output->format, 0, 0, 0);
    Uint8 r, g, b;
    for (int i = 0; i < input->w * input->h; i++)
    {
        SDL_GetRGB(input_pixels[i], input->format, &r, &g, &b);
        output_pixels[i] = (r + g + b) / 3 >= threshold ? white : black;
    }

    // Unlock surfaces.
    SDL_UnlockSurface(input);
    SDL_UnlockSurface(output);

    return output;
}


// Takes a threshold image.
SDL_Surface *invert_filter(SDL_Surface *inputSurface)
{
    // Lock input surface.
    if (SDL_LockSurface(inputSurface))
    {
        errx(EXIT_FAILURE,
             "Error: couldn't lock the surface (invert_filter).");
    }

    // Create output surface.
    SDL_Surface *outputSurface =
        SDL_CreateRGBSurfaceWithFormat(0, inputSurface->w, inputSurface->h, 32,
                                       SDL_PIXELFORMAT_RGBA8888);
    if (!outputSurface)
    {
        errx(EXIT_FAILURE,
             "Error: couldn't create a new surface (invert_filter).");
    }

    // Lock output surface.
    if (SDL_LockSurface(outputSurface))
    {
        errx(EXIT_FAILURE,
             "Error: couldn't lock the surface (invert_filter).");
    }

    // Process each pixels.
    SDL_Color c;
    SDL_PixelFormat *inputFormat = inputSurface->format;
    SDL_PixelFormat *outputFormat = outputSurface->format;
    Uint32 *inputPixels = inputSurface->pixels;
    Uint32 *outputPixels = outputSurface->pixels;
    Uint32 black = SDL_MapRGB(outputFormat, 0, 0, 0);
    Uint32 white = SDL_MapRGB(outputFormat, 255, 255, 255);
    for (int i = 0; i < inputSurface->w * inputSurface->h; i++)
    {
        // Get pixel color.
        SDL_GetRGB(inputPixels[i], inputFormat, &c.r, &c.g, &c.b);

        // Set output pixel.
        outputPixels[i] = c.r == 0 ? white : black;
    }

    // Unlock surfaces.
    SDL_UnlockSurface(inputSurface);
    SDL_UnlockSurface(outputSurface);

    return outputSurface;
}

// Takes a grayscale image.
SDL_Surface *gaussian_filter(SDL_Surface *inputSurface)
{
    // Lock input surface.
    if (SDL_LockSurface(inputSurface))
    {
        errx(EXIT_FAILURE, "Error: couldn't lock the surface (sobel_filter).");
    }

    // Create output surface.
    SDL_Surface *outputSurface =
        SDL_CreateRGBSurfaceWithFormat(0, inputSurface->w, inputSurface->h,
                                       32, SDL_PIXELFORMAT_RGBA8888);
    if (!outputSurface)
    {
        errx(EXIT_FAILURE,
             "Error: couldn't create a new surface (kernel_sobel_filter).");
    }

    // Lock output surface.
    if (SDL_LockSurface(outputSurface))
    {
        errx(EXIT_FAILURE,
             "Error: couldn't lock the surface (kernel_sobel_filter).");
    }

    int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1},
    };

    // Process image.
    SDL_Color c;
    SDL_PixelFormat *inputFormat = inputSurface->format;
    SDL_PixelFormat *outputFormat = outputSurface->format;
    Uint32 *inputPixels = inputSurface->pixels;
    Uint32 *outputPixels = outputSurface->pixels;
    int sum, magnitude;
    for (int x = 0; x < inputSurface->h; x++)
    {
        for (int y = 0; y < inputSurface->w; y++)
        {
            sum = 0;
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if (x + i < 0 || x + i >= inputSurface->h ||
                        y + j < 0 || y + j >= inputSurface->w)
                    {
                        continue;
                    }

                    // Get pixel color.
                    SDL_GetRGB(inputPixels[(x + i) * inputSurface->w + y + j],
                               inputFormat, &c.r, &c.g, &c.b);

                    // Apply kernels for X and Y directions.
                    sum += c.r * kernel[i + 1][j + 1];
                }
            }

            // Calculate the magnitude of the gradient.
            magnitude = sum / 16;

            // Set output pixel with the magnitude as grayscale color.
            outputPixels[x * inputSurface->w + y] =
                SDL_MapRGB(outputFormat, magnitude, magnitude, magnitude);
        }
    }

    // Unlock surfaces.
    SDL_UnlockSurface(inputSurface);
    SDL_UnlockSurface(outputSurface);

    return outputSurface;
}

// Takes a grayscale image.
SDL_Surface *sobel_filter(SDL_Surface *inputSurface)
{
    // Lock input surface.
    if (SDL_LockSurface(inputSurface))
    {
        errx(EXIT_FAILURE, "Error: couldn't lock the surface (sobel_filter).");
    }

    // Create output surface.
    SDL_Surface *outputSurface =
        SDL_CreateRGBSurfaceWithFormat(0, inputSurface->w, inputSurface->h, 32,
                                       SDL_PIXELFORMAT_RGBA8888);
    if (!outputSurface)
    {
        errx(EXIT_FAILURE,
             "Error: couldn't create a new surface (kernel_sobel_filter).");
    }

    // Lock output surface.
    if (SDL_LockSurface(outputSurface))
    {
        errx(EXIT_FAILURE,
             "Error: couldn't lock the surface (kernel_sobel_filter).");
    }

    int kernelX[3][3] = {
        {1, 0, -1},
        {2, 0, -2},
        {1, 0, -1},
    };

    int kernelY[3][3] = {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1},
    };

    // Process image.
    SDL_Color c;
    SDL_PixelFormat *inputFormat = inputSurface->format;
    SDL_PixelFormat *outputFormat = outputSurface->format;
    Uint32 *inputPixels = inputSurface->pixels;
    Uint32 *outputPixels = outputSurface->pixels;
    int sumX, sumY, magnitude;
    for (int x = 0; x < inputSurface->h; x++)
    {
        for (int y = 0; y < inputSurface->w; y++)
        {
            sumX = sumY = 0;
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if (x + i < 0 || x + i >= inputSurface->h ||
                        y + j < 0 || y + j >= inputSurface->w)
                    {
                        continue;
                    }

                    // Get pixel color.
                    SDL_GetRGB(inputPixels[(x + i) * inputSurface->w + y + j],
                               inputFormat, &c.r, &c.g, &c.b);

                    // Apply kernels for X and Y directions.
                    sumX += c.r * kernelX[i + 1][j + 1];
                    sumY += c.r * kernelY[i + 1][j + 1];
                }
            }

            // Calculate the magnitude of the gradient.
            magnitude = (int)sqrt((double)(sumX * sumX + sumY * sumY));

            if (magnitude > 255)
            {
                magnitude = 255;
            }

            // Set output pixel with the magnitude as grayscale color.
            outputPixels[x * inputSurface->w + y] =
                SDL_MapRGB(outputFormat, magnitude, magnitude, magnitude);
        }
    }

    // Unlock surfaces.
    SDL_UnlockSurface(inputSurface);
    SDL_UnlockSurface(outputSurface);

    return outputSurface;
}

// Takes a sobel image and threshold image.
void draw_sudoku_grid_outline(SDL_Surface *input)
{
    Uint32 blue = SDL_MapRGB(input->format, 0, 0, 255);
    Uint32 green = SDL_MapRGB(input->format, 0, 255, 0);

    int max_square_size = 0;
    int max_square_y = 0;
    int max_square_x = 0;

    // Search biggest square.
    for (int y = 0; y < input->h; y++)
    {
        for (int x = 0; x < input->w; x++)
        {
            // Check if the pixel is green.
            if (((Uint32 *)input->pixels)[y * input->w + x] == green)
            {
                int size = 1;
                while (x + size < input->w && y + size < input->h && ((Uint32 *)input->pixels)[(y + size) * input->w + x] == green && ((Uint32 *)input->pixels)[y * input->w + x + size] == green)
                {
                    size++;
                }

                if (size > max_square_size)
                {
                    max_square_size = size;
                    max_square_y = y;
                    max_square_x = x;
                }
            }
        }
    }

    // Draw the blue outline around the biggest square on the output surface.
    for (int i = 0; i < max_square_size; i++)
    {

        int ny = max_square_y;
        int nx = max_square_x + i;
        if (nx >= 0 && nx < input->w && ny >= 0 && ny < input->h)
        {
            ((Uint32 *)input->pixels)[ny * input->w + nx] = blue;
        }

        ny = max_square_y + max_square_size - 1;
        if (nx >= 0 && nx < input->w && ny >= 0 && ny < input->h)
        {
            ((Uint32 *)input->pixels)[ny * input->w + nx] = blue;
        }

        ny = max_square_y + i;
        nx = max_square_x;
        if (nx >= 0 && nx < input->w && ny >= 0 && ny < input->h)
        {
            ((Uint32 *)input->pixels)[ny * input->w + nx] = blue;
        }

        nx = max_square_x + max_square_size - 1;
        if (nx >= 0 && nx < input->w && ny >= 0 && ny < input->h)
        {
            ((Uint32 *)input->pixels)[ny * input->w + nx] = blue;
        }
    }
}

SDL_Surface *hough_transform_filter(SDL_Surface *input)
{
    SDL_Surface *inputSurface = input;
    int threshold = 200;

    // Lock input surface.
    if (SDL_LockSurface(inputSurface))
    {
        errx(EXIT_FAILURE,
             "Error: couldn't lock the surface (hough_transform).");
    }

    // Create output surface.
    SDL_Surface *outputSurface =
        SDL_CreateRGBSurfaceWithFormat(0, inputSurface->w, inputSurface->h, 32,
                                       SDL_PIXELFORMAT_RGBA8888);
    if (!outputSurface)
    {
        errx(EXIT_FAILURE,
             "Error: couldn't create a new surface (hough_transform).");
    }

    // Lock output surface.
    if (SDL_LockSurface(outputSurface))
    {
        errx(EXIT_FAILURE,
             "Error: couldn't lock the surface (hough_transform).");
    }

    // Copy input to output.
    for (int i = 0; i < inputSurface->w * inputSurface->h; i++)
    {
        SDL_Color c;
        SDL_GetRGB(((Uint32 *)inputSurface->pixels)[i],
                   inputSurface->format, &c.r, &c.g, &c.b);
        ((Uint32 *)outputSurface->pixels)[i] =
            SDL_MapRGB(outputSurface->format, c.r, c.g, c.b);
    }

    // Create hough space.
    int houghSpaceHeight = (int)sqrt(pow(inputSurface->w, 2) +
                                     pow(inputSurface->h, 2));
    int houghSpaceWidth = 180;
    int **houghSpace = (int **)malloc(houghSpaceHeight * sizeof(int *));
    for (int i = 0; i < houghSpaceHeight; i++)
    {
        houghSpace[i] = (int *)calloc(houghSpaceWidth, sizeof(int));
    }

    // Update hough space.
    for (int x = 0; x < inputSurface->w; x++)
    {
        for (int y = 0; y < inputSurface->h; y++)
        {
            SDL_Color c;
            SDL_GetRGB(((Uint32 *)inputSurface->pixels)
                           [y * inputSurface->w + x],
                       inputSurface->format, &c.r, &c.g, &c.b);
            if (c.r == 255)
            {
                // White pixel.
                for (int theta = 0; theta < 180; theta++)
                {
                    double radians = theta * M_PI / 180;
                    int rho = (int)(x * cos(radians) + y * sin(radians));
                    if (rho >= 0 && rho < houghSpaceHeight)
                    {
                        houghSpace[rho][theta] += 1;
                    }
                }
            }
        }
    }

    // Draw potential lines.
    Uint32 green = SDL_MapRGB(outputSurface->format, 0, 255, 0);
    Uint32 white = SDL_MapRGB(outputSurface->format, 255, 255, 255);
    for (int rho = 0; rho < houghSpaceHeight; rho++)
    {
        for (int theta = 0; theta < 180; theta++)
        {
            int value = houghSpace[rho][theta];
            if (value >= threshold)
            {
                double radians = theta * M_PI / 180;
                for (int x = 0; x < inputSurface->w; x++)
                {
                    int y = (int)((rho - x * cos(radians)) / sin(radians));
                    if (y >= 0 && y < inputSurface->h)
                    {
                        if (((Uint32 *)outputSurface->pixels)[y * outputSurface->w + x] != white)
                            continue;
                        ((Uint32 *)outputSurface->pixels)
                            [y * outputSurface->w + x] = green;
                    }
                }

                for (int y = 0; y < inputSurface->h; y++)
                {
                    int x = (int)((rho - y * sin(radians)) / cos(radians));
                    if (x >= 0 && x < inputSurface->w)
                    {
                        if (((Uint32 *)outputSurface->pixels)[y * outputSurface->w + x] != white)
                            continue;
                        ((Uint32 *)outputSurface->pixels)
                            [y * outputSurface->w + x] = green;
                    }
                }
            }
        }
    }

    // Draw biggest square.
    draw_sudoku_grid_outline(outputSurface);

    // Free.
    for (int i = 0; i < houghSpaceHeight; i++)
    {
        free(houghSpace[i]);
    }

    free(houghSpace);

    // Unlock surfaces.
    SDL_UnlockSurface(input);
    SDL_UnlockSurface(outputSurface);

    return outputSurface;
}


SDL_Surface *rotate_filter(SDL_Surface *inputSurface, double angle)
{
    // Lock input surface.
    if (SDL_LockSurface(inputSurface))
    {
        errx(EXIT_FAILURE,
             "Error: couldn't lock the surface (grayscale_filter).");
    }

    // Set the rotation angle (in radians) based on user input.
    angle = angle * M_PI / 180;

    // Calculate the size of the rotated image.
    int newWidth = fabs(inputSurface->w * cos(angle)) +
                   fabs(inputSurface->h * sin(angle));
    int newHeight = fabs(inputSurface->w * sin(angle)) +
                    fabs(inputSurface->h * cos(angle));

    // Calculate the pivot point for rotation (center of the image).
    int pivotX = inputSurface->w / 2;
    int pivotY = inputSurface->h / 2;

    // Create output surface.
    SDL_Surface *outputSurface =
        SDL_CreateRGBSurfaceWithFormat(0, newWidth, newHeight, 32,
                                       SDL_PIXELFORMAT_RGBA8888);
    if (!outputSurface)
    {
        errx(EXIT_FAILURE,
             "Error: couldn't create a new surface (grayscale_filter).");
    }

    // Lock output surface.
    if (SDL_LockSurface(outputSurface))
    {
        errx(EXIT_FAILURE,
             "Error: couldn't lock the surface (grayscale_filter).");
    }

    for (int x = 0; x < newWidth; x++)
    {
        for (int y = 0; y < newHeight; y++)
        {
            int srcX = (int)((x - outputSurface->w / 2) * cos(angle) -
                             (y - outputSurface->h / 2) * sin(angle) + pivotX);
            int srcY = (int)((x - outputSurface->w / 2) * sin(angle) +
                             (y - outputSurface->h / 2) * cos(angle) + pivotY);

            if (srcX >= 0 && srcX < inputSurface->w

                && srcY >= 0 && srcY < inputSurface->h)
            {
                SDL_Color c;
                Uint32 pixel = ((Uint32 *)inputSurface->pixels)
                    [srcY * inputSurface->w + srcX];
                SDL_GetRGB(pixel, inputSurface->format, &c.r, &c.g, &c.b);
                ((Uint32 *)outputSurface->pixels)
                    [y * outputSurface->w + x] = SDL_MapRGB(
                        outputSurface->format, c.r, c.g, c.b);
            }
            else
            {
                ((Uint32 *)outputSurface->pixels)
                    [y * outputSurface->w + x] = SDL_MapRGB(
                        outputSurface->format, 0, 0, 0);
            }
        }
    }

    return outputSurface;
}
