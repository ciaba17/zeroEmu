#include "../include/Renderer.hpp"

Renderer::Renderer(SDL_Renderer* renderer) {}

void Renderer::render() {
    // Set renderer color white (background)
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // bianco

    // Clear screen
    SDL_RenderClear(renderer);
            



                
    // Present renderer
    SDL_RenderPresent(renderer);
}
