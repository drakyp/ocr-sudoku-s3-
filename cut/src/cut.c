#include "cut.h"

void cut_and_save(SDL_Surface *surface, size_t n)
{
    // Create output directory.
    mkdir("output", S_IRWXU | S_IRWXG | S_IRWXO);

    size_t imageWidth = surface->w / n;
    size_t imageHeight = surface->h / n;

    // For each cell.
    for (size_t x = 0; x < n; x++)
    {
      for (size_t y = 0; y < n; y++)
      {
         // Create image.
         SDL_Surface *image = SDL_CreateRGBSurface(0, imageWidth, imageHeight,
			surface->format->BitsPerPixel, surface->format->Rmask, 
			surface->format->Gmask, surface->format->Bmask, 
			surface->format->Amask);

            // Lock image.
            SDL_LockSurface(image);

            // Pixels to read.
            Uint32 *surfacePixels = surface->pixels;

            // Pixels to write to.
            Uint32 *imagePixels = image->pixels;

            for (size_t i = 0; i < imageHeight; i++)
            {
                for (size_t j = 0; j < imageWidth; j++)
                {
                    // Calculate the source pixel index and copy the pixel.
                  size_t sourceIndex = (x * imageWidth + j) + 
			     (y * imageHeight + i) * surface->w;
                  imagePixels[i * imageWidth + j] = surfacePixels[sourceIndex];
                }
            }

            // Check if empty.
            int midX = image->w / 2;
            int midY = image->h / 2;

            int isValid = 0;
            Uint32 white = SDL_MapRGB(image->format, 255, 255, 255);
            for (int i = -20; i <= 20; i++)
            {
                for (int j = -20; j <= 20; j++)
                {
                    if (imagePixels[midX * image->w + midY + 
				  i * image->w + j] == white)
                    {
                        isValid = 1;
                        break;
                    }
                }

                if (isValid)
                {
                    break;
                }
            }

            if (isValid)
            {
                // Save image.
                char filename[100];
                sprintf(filename, "output/output_%zu-%zu.png", x, y);
                IMG_SavePNG(image, filename);
            }

            // Unlock image.
            SDL_UnlockSurface(image);
            SDL_FreeSurface(image);
        }
    }
}
