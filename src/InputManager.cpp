#include "../include/InputManager.hpp"

void InputManager::update(const SDL_Event& e) {
    if (e.type == SDLK_DOWN) {
        keyStates[e.key.keysym.scancode] = true;
        keyReleased[e.key.keysym.scancode] = false;
    }

    if (e.type == SDLK_UP) {
        keyStates[e.key.keysym.scancode] = false;
        keyReleased[e.key.keysym.scancode] = true;
    }

    if (e.type == SDLK_LEFT) {
        keyStates[e.key.keysym.scancode] = false;
        keyReleased[e.key.keysym.scancode] = true;
    }

    if (e.type == SDLK_RIGHT) {
        keyStates[e.key.keysym.scancode] = false;
        keyReleased[e.key.keysym.scancode] = true;
    }
}

bool InputManager::isKeyPressed(SDL_Scancode key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

bool InputManager::isKeyReleased(SDL_Scancode key) const {
    auto it = keyReleased.find(key);
    return it != keyReleased.end() && it->second;
}