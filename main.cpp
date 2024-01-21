#include <iostream>
#include <thread>

#include "window.h"
#include "singleplayer.h"
#include "multiplayer.h"

#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>

int main(int argc, char *argv[]) {
    // auto ids = SDL_GetGamepads(&pads);
    // auto joyId = SDL_OpenGamepad(ids[0]);
    // std::cout << SDL_GetGamepadName(joyId) << std::endl;
    // std::cout << SDL_GetGamepadPath(joyId) << std::endl;

    Window window("New_Pong");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io{ImGui::GetIO()};
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    //    io.FontGlobalScale = scale;
    //    ImGui::GetStyle().ScaleAllSizes(scale);
    ImGui_ImplSDL3_InitForSDLRenderer(window.getWindow(), window.getRenderer());
    ImGui_ImplSDLRenderer3_Init(window.getRenderer());
    // ImGui_ImplSDL3_SetGamepad(joyId);
    while (true) {
        GameBase * game = nullptr;
        SDL_Event event;
        while (window.pollEvent(event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                break;
        }
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(window.getWidth() / 3, window.getHeight() / 4));
        ImGui::SetNextWindowSize(ImVec2(window.getWidth() / 3, window.getHeight() / 2));
        ImGui::Begin("Main menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);
        if (ImGui::Button("Singleplayer"))
            game = new Singleplayer(window);
        if (ImGui::Button("Multiplayer"))
            game = new Multiplayer(window);
        if (ImGui::Button("Quit"))
            break;
        ImGui::End();
        window.clear(0, 0, 0);
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
        if (game) {
            game->run();
            delete game;
        } else {
            window.display();
        }
    }
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}
