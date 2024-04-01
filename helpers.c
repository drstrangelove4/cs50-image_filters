// Headers
#include "helpers.h"
#include <math.h>
#include <stdio.h>

// --------------------------------------------------------------------------------------------------------------------

// Signatures
void copy_buffer(int height, int width, RGBTRIPLE image[height][width],
                 RGBTRIPLE buffer[height][width]);

// --------------------------------------------------------------------------------------------------------------------

// Convert image to grayscale.
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[height][width];

    // Iterates over every "pixel" found the image, takes the average of the RGB values and sets
    // each value to that average.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float average =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            buffer[i][j] = (RGBTRIPLE){
                .rgbtBlue = average,
                .rgbtGreen = average,
                .rgbtRed = average,
            };
        }
    }

    // Copy the buffer into the image array
    copy_buffer(height, width, image, buffer);
}

// --------------------------------------------------------------------------------------------------------------------

// Reflect image horizontally.
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Reverse the order of the pixels in row I. This should flip the image horizontally.
    for (int i = 0; i < height; i++)
    {
        RGBTRIPLE buffer[width];

        // Copy row I into the buffer.
        for (int j = 0; j < width; j++)
        {
            buffer[j] = image[i][j];
        }

        // Copy the buffer into the image array
        for (int x = 0; x < width; x++)
        {
            image[i][x] = buffer[width - 1 - x];
        }
    }
}

// --------------------------------------------------------------------------------------------------------------------

// Blur image.
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[height][width];

    // Takes the average of neighboring pixel RGB values and applies that to every pixel in the
    // bitmap.
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Total the RGB values of neighbors and the total valid neighbors.
            float blue_total = 0;
            float green_total = 0;
            float red_total = 0;
            float count = 0;

            // We want a 3 x 3 grid around the current pixel.
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    // Check for valid X and Y values - we don't want values outside of the image
                    // array.
                    if (y + i >= 0 && y + i < height)
                    {
                        if (x + j >= 0 && x + j < width)
                        {
                            // Add the values to totals
                            count++;
                            blue_total += image[y + i][x + j].rgbtBlue;
                            green_total += image[y + i][x + j].rgbtGreen;
                            red_total += image[y + i][x + j].rgbtRed;
                        }
                    }
                }
            }

            // Add the blurred pixel to the buffer

            buffer[y][x] = (RGBTRIPLE){
                .rgbtBlue = round(blue_total / count),
                .rgbtGreen = round(green_total / count),
                .rgbtRed = round(red_total / count),
            };
        }
    }

    // Copy the buffer into the image array
    copy_buffer(height, width, image, buffer);
}

// --------------------------------------------------------------------------------------------------------------------

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // Multiplcation constants.
    const int GX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    const int GY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    RGBTRIPLE buffer[height][width];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int x_totals[] = {0, 0, 0};
            int y_totals[] = {0, 0, 0};

            // Calcualte GX and GY values
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    // Check for valid X and Y values - we don't want values outside of the image
                    // array.
                    if (y + i >= 0 && y + i < height)
                    {
                        if (x + j >= 0 && x + j < width)
                        {
                            // Get colour values of a pixel
                            int colour_values[] =
                                {
                                    image[y + i][x + j].rgbtBlue,
                                    image[y + i][x + j].rgbtGreen,
                                    image[y + i][x + j].rgbtRed,
                                };

                            // Get GX and GY value
                            for (int a = 0; a < 3; a++)
                            {
                                x_totals[a] += colour_values[a] * GX[i + 1][j + 1];
                                y_totals[a] += colour_values[a] * GY[i + 1][j + 1];
                            }
                        }
                    }
                }
            }

            // Populate results
            int results[3];
            for (int j = 0; j < 3; j++)
            {
                results[j] = round(sqrt(x_totals[j] * x_totals[j] + (y_totals[j] * y_totals[j])));

                // Range check result
                if (results[j] > 255)
                {
                    results[j] = 255;
                }
                if (results[j] < 0)
                {
                    results[j] = 0;
                }
            }

            // Add the changed pixel to the buffer
            buffer[y][x] = (RGBTRIPLE){
                .rgbtBlue = results[0],
                .rgbtGreen = results[1],
                .rgbtRed = results[2],
            };
        }
    }

    // Copy the buffer into the image array
    copy_buffer(height, width, image, buffer);
}

// --------------------------------------------------------------------------------------------------------------------

// Copies the contents of the buffer array into the image array.
void copy_buffer(int height, int width, RGBTRIPLE image[height][width],
                 RGBTRIPLE buffer[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = buffer[i][j];
        }
    }
    return;
}
