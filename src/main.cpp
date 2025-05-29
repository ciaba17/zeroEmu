#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include "../include/Globals.hpp"
#include "../include/WindowManager.hpp"
#include "../include/MenuManager.hpp"
#include "../include/TextureManager.hpp"
#include "../include/InputManager.hpp"

Uint32 frameStart;

void rendering(SDL_Renderer* renderer, WindowManager& WM, MenuManager& menu, TextureManager& TM);

int main() {
    // Create the window manager
    WindowManager WM;

    // Initialize SDL and exit if initialization fail
    if (!WM.init("ZeroEmu")) return 0;
    
    // Create all objects
    TextureManager TM(WM.renderer);
    MenuManager menu(&TM.textures);
    InputManager input;

    

    // Event loop exit flag
    bool quit = false; 
    
    // Event loop
    while(!quit) {
        frameStart = SDL_GetTicks();
        SDL_Event e;

        while (SDL_PollEvent(&e)) {
            input.update(e);
        }

        // User requests quit
        if(e.type == SDL_QUIT) {
            quit = true;
        }

        menu.handleInput(input);
        rendering(WM.renderer, WM, menu, TM);



        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Destroy renderer
    SDL_DestroyRenderer(WM.renderer);

    // Destroy window
    SDL_DestroyWindow(WM.window);
    
    // Quit SDL
    IMG_Quit();
    SDL_Quit();

    return 0;
}


void rendering(SDL_Renderer* renderer, WindowManager& WM, MenuManager& menu, TextureManager& TM) {
    // Set renderer color white (background)
    SDL_SetRenderDrawColor(WM.renderer, 255, 255, 255, 255); // bianco

    // Clear screen
    SDL_RenderClear(renderer);
    
    if (consolesMenu) {
        menu.menuRender(renderer);
    }
    else if (gamesMenu) {

    }
    
    menu.menuRender(renderer);

    SDL_RenderPresent(renderer);
}