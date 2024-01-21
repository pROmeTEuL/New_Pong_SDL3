#include "singleplayer.h"
#include <string>
#include <thread>

Singleplayer::Singleplayer(Window &window)
    : m_window(window)
{
    m_player = new Player(ID::SINGLEPLAYER, window.getWidth(), window.getHeight());
    m_ball = new Ball(window.getWidth(), window.getHeight(), m_player->getID());
}

Singleplayer::~Singleplayer()
{
    delete m_player;
    delete m_ball;
}

void Singleplayer::processEvent(SDL_Event &event)
{
    m_player->processEvent(event, *m_ball);
}

void Singleplayer::run()
{
    while (m_playing) {
        SDL_Event event;
        while (m_window.pollEvent(event)) {
            processEvent(event);
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.keysym.sym == SDLK_ESCAPE)
                m_paused = !m_paused;
        }
        if (!m_paused) {
            update();
        }
        draw();
        std::this_thread::sleep_for(std::chrono::microseconds{16667});
    }
}

void Singleplayer::update()
{
    m_player->update(m_window.getWidth());
    m_ball->update(*m_player, m_window.getWidth(), m_window.getHeight());
}

void Singleplayer::draw()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    std::string score = "Score: " + std::to_string(m_player->getScore());
    ImGui::Begin("Score", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
    m_window.clear(0, 0, 0);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::Text(score.c_str());
    ImGui::PopStyleColor();
    ImGui::End();
    m_player->draw(m_window.getRenderer());
    m_ball->draw(m_window.getRenderer());
    if (m_paused) {
        //         ImGui::ShowDemoWindow(&p_open);
        ImGui::SetNextWindowPos(ImVec2(m_window.getWidth() / 3, m_window.getHeight() / 4));
        ImGui::SetNextWindowSize(ImVec2(m_window.getWidth() / 3, m_window.getHeight() / 2));
        ImGui::Begin("h", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);
        if (ImGui::Button("Resume"))
            m_paused = false;
        if (ImGui::Button("Quit"))
            m_playing = false;
        // UI goes here
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
    m_window.display();
}
