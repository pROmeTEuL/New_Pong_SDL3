#include <iostream>
#include <thread>

#include "window.h"
#include "player.h"
#include "ball.h"

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
    Player player1(ID::PLAYER1, window.getWidth(), window.getHeight());
    Player player2(ID::PLAYER2, window.getWidth(), window.getHeight());
    Ball ball(window.getWidth(), window.getHeight());

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
    bool p_open = true;
    while (1) {
        int finished = 0;
        SDL_Event event;
        while (window.pollEvent(event)) {
            player1.processEvent(event, ball);
            player2.processEvent(event, ball);
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                finished = 1;
                break;
            }
        }
        if (finished) {
            break;
        }

        player1.update(window.getWidth());
        player2.update(window.getWidth());
        ball.update(player1, window.getWidth(), window.getHeight());
        ball.update(player2, window.getWidth(), window.getHeight());

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // static bool use_work_area = false;
        // const ImGuiViewport* viewport = ImGui::GetMainViewport();
        // ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
        // ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
//         ImGui::ShowDemoWindow(&p_open);
        ImGui::Begin("h", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
        std::string score1 = "Player1 score=" + std::to_string(player1.getScore());
        std::string score2 = "Player2 score=" + std::to_string(player2.getScore());
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 68, 84, 255));
        ImGui::Text(score1.c_str());
        ImGui::PopStyleColor();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 193, 226, 255));
        ImGui::Text(score2.c_str());
        ImGui::PopStyleColor();
        // UI goes here
        ImGui::End();

        ImGui::Render();

        window.clear(0, 0, 0);
        if (ball.getOwner() == PLAYER1) {
            player2.draw(window.getRenderer());
            player1.draw(window.getRenderer());
        } else {
            player1.draw(window.getRenderer());
            player2.draw(window.getRenderer());
        }
        ball.draw(window.getRenderer());
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
        window.display();

        std::this_thread::sleep_for(std::chrono::microseconds{16667});
    }
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    // SDL_CloseGamepad(joyId);
    window.~Window();
}
