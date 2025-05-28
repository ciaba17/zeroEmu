#include "../include/InputManager.hpp"

void InputManager::update(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        keyStates[e.key.keysym.scancode] = true;
        keyReleased[e.key.keysym.scancode] = false;
    }

    if (e.type == SDL_KEYUP) {
        keyStates[e.key.keysym.scancode] = false;
        keyReleased[e.key.keysym.scancode] = true;
    }

    if (e.type == SDLK_LEFT) {
        keyStates[e.key.keysym.scancode] = true;
        keyReleased[e.key.keysym.scancode] = false;
    }

    if (e.type == SDLK_RIGHT) {
        keyStates[e.key.keysym.scancode] = true;
        keyReleased[e.key.keysym.scancode] = false;
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