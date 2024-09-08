#include "SDL.h"
#include<stdio.h>


// Function to draw a highlighted box
void drawHighlight(SDL_Renderer* renderer, int x, int y, int width, int height) {
    // Set the color for the highlight (e.g., yellow)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    // Draw the highlight box
    SDL_Rect highlightRect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &highlightRect);
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Chessboard Highlight Example", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Chessboard parameters
    int squareSize = 50;
    int rows = 8;
    int cols = 8;

    // Main loop
    int quit = 0;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                // Handle mouse click events
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Calculate the clicked square position
                int clickedCol = mouseX / squareSize;
                int clickedRow = mouseY / squareSize;

                // Draw the chessboard
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        int x = j * squareSize;
                        int y = i * squareSize;

                        // Draw a chessboard square
                        SDL_SetRenderDrawColor(renderer, (i + j) % 2 == 0 ? 255 : 0, (i + j) % 2 == 0 ? 255 : 0, (i + j) % 2 == 0 ? 255 : 0, 255);
                        SDL_Rect squareRect = { x, y, squareSize, squareSize };
                        SDL_RenderFillRect(renderer, &squareRect);

                        // Highlight the clicked square
                        if (i == clickedRow && j == clickedCol) {
                            drawHighlight(renderer, x, y, squareSize, squareSize);
                        }
                    }
                }

                // Present the renderer
                SDL_RenderPresent(renderer);
            }
        }
    }

    // Destroy window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL
    SDL_Quit();

    return 0;
}
