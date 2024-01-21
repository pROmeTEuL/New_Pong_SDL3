#include "gamebase.h"
#include <thread>

GameBase::GameBase(Window &window)
    : m_window(window)
{
}

bool GameBase::run()
{
    bool quit_Entirely = false;
    while (m_playing) {
        processEvent();
        if (!m_paused)
            update(m_window.getWidth(), m_window.getHeight());
        quit_Entirely = draw();
        std::this_thread::sleep_for(std::chrono::microseconds{16667});
    }
    return quit_Entirely;
}

void GameBase::processEvent()
{
    SDL_Event event;
    while (m_window.pollEvent(event)) {
        processObjectEvent(event);
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.keysym.sym == SDLK_ESCAPE)
            m_paused = !m_paused;
    }
}

bool GameBase::draw()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    m_window.clear(0, 0, 0);
    drawScore();
    drawObjects(m_window.getRenderer());
    if (m_paused) {
        //         ImGui::ShowDemoWindow(&p_open);
        ImGui::SetNextWindowPos(ImVec2(m_window.getWidth() / 3, m_window.getHeight() / 4));
        ImGui::SetNextWindowSize(ImVec2(m_window.getWidth() / 3, m_window.getHeight() / 2));
        ImGui::Begin("h", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);
        if (ImGui::Button("Resume"))
            m_paused = false;
        if (ImGui::Button("Main Menu")) {
            m_playing = false;
            return false;
        }
        if (ImGui::Button("Quit")) {
            m_playing = false;
            return true;
        }
        // UI goes here
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
    m_window.display();
    return false;
}
