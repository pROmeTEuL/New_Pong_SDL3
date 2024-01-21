#include "singleplayer.h"

Singleplayer::Singleplayer(Window &window)
    : m_window(window)
{
    m_player = new Player(ID::SINGLEPLAYER, window.getWidth(), window.getHeight());
    m_ball = new Ball(window.getWidth(), window.getHeight(), m_player->getID());
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
    ImGui::SetNextWindowSize(ImVec2(m_window.getWidth() / 3, m_window.getHeight() / 2));
    m_player->draw(m_window.getRenderer());
    m_ball->draw(m_window.getRenderer());
    if (m_paused) {
        //         ImGui::ShowDemoWindow(&p_open);
        ImGui::SetNextWindowPos(ImVec2(m_window.getWidth() / 3, m_window.getHeight() / 4));
        ImGui::SetNextWindowSize(ImVec2(m_window.getWidth() / 3, m_window.getHeight() / 2));
        ImGui::Begin("h", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
        //        std::string score1 = "Player1 score=" + std::to_string(player1.getScore());
        //        std::string score2 = "Player2 score=" + std::to_string(player2.getScore());
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 68, 84, 255));
        ImGui::Text(score1.c_str());
        ImGui::PopStyleColor();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 193, 226, 255));
        ImGui::Text(score2.c_str());
        ImGui::PopStyleColor();
        // UI goes here
        ImGui::End();

        ImGui::Render();
    }
}
