#include <iostream>
#include <thread>

#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>
#include <memory>

#include "window.h"
#include "singleplayer.h"
#include "multiplayer.h"
#include "menu.h"

int main(int argc, char *argv[]) {
    // auto ids = SDL_GetGamepads(&pads);
    // auto joyId = SDL_OpenGamepad(ids[0]);
    // std::cout << SDL_GetGamepadName(joyId) << std::endl;
    // std::cout << SDL_GetGamepadPath(joyId) << std::endl;

    auto window = std::make_shared<Window>("New_Pong");
    Menu::instance().setWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io{ImGui::GetIO()};
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    //    io.FontGlobalScale = scale;
    //    ImGui::GetStyle().ScaleAllSizes(scale);
    ImGui_ImplSDL3_InitForSDLRenderer(window->getWindow(), window->getRenderer());
    ImGui_ImplSDLRenderer3_Init(window->getRenderer());
    // ImGui_ImplSDL3_SetGamepad(joyId);
    bool quit = false;
    Menu::instance().setQuitCallback([&quit]{
        quit = true;
    });
    GameBase * game = nullptr;
    Menu::instance().setSinglePlayerCallback([&game, &window]{
        delete game;
        game = new Singleplayer(window);
    });
    Menu::instance().setMultiPlayerCallback([&game, &window]{
        delete game;
        game = new Multiplayer(window);
    });
    while (!quit) {
        SDL_Event event;
        while (window->pollEvent(event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                break;
        }
        window->clear(0, 0, 0);
        Menu::instance().draw();
        if (game) {
            game->run();
            delete game;
            game = nullptr;
        } else {
            window->display();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}
