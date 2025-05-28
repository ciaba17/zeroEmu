#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>

class InputManager {
private:
    std::unordered_map<SDL_Scancode, bool> keyStates; // Current key states
    std::unordered_map<SDL_Scancode, bool> keyReleased;
public:
    void update(const SDL_Event& e);
    bool isKeyPressed(SDL_Scancode key) const;
    bool isKeyReleased(SDL_Scancode key) const;
};