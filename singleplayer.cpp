#include "singleplayer.h"

Singleplayer::Singleplayer(Window &window) : GameBase(window)
{
    m_player = new Player(SINGLEPLAYER, window.getWidth(), window.getHeight());
    m_ball = new Ball(window.getWidth(), window.getHeight(), m_player->getID());
}

Singleplayer::~Singleplayer()
{
    delete m_player;
    delete m_ball;
}

void Singleplayer::processObjectEvent(SDL_Event &event)
{
    m_player->processEvent(event, *m_ball);
}

void Singleplayer::update(int width, int height)
{
    m_player->update(width);
    m_ball->update(*m_player, width, height);
}

void Singleplayer::drawObjects(SDL_Renderer *renderer)
{
    m_player->draw(renderer);
    m_ball->draw(renderer);
}

void Singleplayer::drawScore()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Score", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::Text("Score: %d", m_player->getScore());
    ImGui::PopStyleColor();
    ImGui::End();
}
