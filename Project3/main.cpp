#include "Game.h"
#include "MenuState.h"
#include <iostream>

int main() {
    try {
        Game game;
        game.pushState(std::make_unique<MenuState>(&game));
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

