#include "helpers.h"
#include <math.h>
#include <stdio.h>

int min(int a, int b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            int averageRGB = round((red + green + blue) / 3.0);
            image[i][j].rgbtRed = averageRGB;
            image[i][j].rgbtGreen = averageRGB;
            image[i][j].rgbtBlue = averageRGB;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            int sepiaRed = round(.393 * red + .769 * green + .189 * blue);
            int sepiaGreen = round(.349 * red + .686 * green + .168 * blue);
            int sepiaBlue = round(.272 * red + .534 * green + .131 * blue);
            image[i][j].rgbtRed = min(sepiaRed, 255);
            image[i][j].rgbtGreen = min(sepiaGreen, 255);
            image[i][j].rgbtBlue = min(sepiaBlue, 255);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            int leftSideRed = image[i][j].rgbtRed;
            int leftSideGreen = image[i][j].rgbtGreen;
            int leftSideBlue = image[i][j].rgbtBlue;
            int rightSideRed = image[i][width - j - 1].rgbtRed;
            int rightSideGreen = image[i][width - j - 1].rgbtGreen;
            int rightSideBlue = image[i][width - j - 1].rgbtBlue;
            int tempLeftRed = leftSideRed;
            int tempLeftGreen = leftSideGreen;
            int tempLeftBlue = leftSideBlue;
            image[i][j].rgbtRed = rightSideRed;
            image[i][j].rgbtGreen = rightSideGreen;
            image[i][j].rgbtBlue = rightSideBlue;
            image[i][width - j - 1].rgbtRed = tempLeftRed;
            image[i][width - j - 1].rgbtGreen = tempLeftGreen;
            image[i][width - j - 1].rgbtBlue = tempLeftBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            int piksele = 1;

            if (j > 0)
            {
                red += image[i][j - 1].rgbtRed;
                green += image[i][j - 1].rgbtGreen;
                blue += image[i][j - 1].rgbtBlue;
                piksele++;
            }

            if (j < width - 1)
            {
                red += image[i][j + 1].rgbtRed;
                green += image[i][j + 1].rgbtGreen;
                blue += image[i][j + 1].rgbtBlue;
                piksele++;
            }

            if (i > 0)
            {
                red += image[i - 1][j].rgbtRed;
                green += image[i - 1][j].rgbtGreen;
                blue += image[i - 1][j].rgbtBlue;
                piksele++;
            }
            if (i < height - 1)
            {
                red += image[i + 1][j].rgbtRed;
                green += image[i + 1][j].rgbtGreen;
                blue += image[i + 1][j].rgbtBlue;
                piksele++;
            }

            if (i > 0 && j > 0)
            {
                red += image[i - 1][j - 1].rgbtRed;
                green += image[i - 1][j - 1].rgbtGreen;
                blue += image[i - 1][j - 1].rgbtBlue;
                piksele++;
            }
            if (i > 0 && j < width - 1)
            {
                red += image[i - 1][j + 1].rgbtRed;
                green += image[i - 1][j + 1].rgbtGreen;
                blue += image[i - 1][j + 1].rgbtBlue;
                piksele++;
            }
            if (i < height - 1 && j > 0)
            {
                red += image[i + 1][j - 1].rgbtRed;
                green += image[i + 1][j - 1].rgbtGreen;
                blue += image[i + 1][j - 1].rgbtBlue;
                piksele++;
            }
            if (i < height - 1 && j < width - 1)
            {
                red += image[i + 1][j + 1].rgbtRed;
                green += image[i + 1][j + 1].rgbtGreen;
                blue += image[i + 1][j + 1].rgbtBlue;
                piksele++;
            }

            copy[i][j].rgbtRed = round((float) red / piksele);
            copy[i][j].rgbtGreen = round((float) green / piksele);
            copy[i][j].rgbtBlue = round((float) blue / piksele);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}

int min(int a, int b)
{
    if (a > b)
    {
        return b;
    }
    return a;
}
