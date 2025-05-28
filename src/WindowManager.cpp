#include "../include/WindowManager.hpp"
#include "../include/Globals.hpp"

bool WindowManager::init(const char* title) {
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized!" << std::endl << "SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    // Initialize SDL IMG
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cerr << "Errore nell'inizializzazione di SDL_image: " << IMG_GetError() << std::endl;
        return false;
    }
    

    // Create window if initialized correctly
    window = SDL_CreateWindow(title, windowPosx, windowPosy, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    // Check if window was created successfully
    if(!window) {
        std::cout << "Window could not be created!" << std::endl << "SDL_Error: " << SDL_GetError() << std::endl;
    }
    else {
        // Create renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        // Check if render was created successfully
        if(!renderer) {
            std::cout << "Renderer could not be created!" << std::endl << "SDL_Error: " << SDL_GetError() << std::endl;
        }
        else {
            // Set the blendmode for the renderer to allow transparency
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); 
            return true; // Return success if everything initialized correctly
        }
    }
}