#include "multiplayer.h"

Multiplayer::Multiplayer(const std::shared_ptr<Window> &window) : GameBase(window)
{
    m_player1 = new Player(PLAYER1, window->getWidth(), window->getHeight());
    m_player2 = new Player(PLAYER2, window->getWidth(), window->getHeight());
    m_ball = new Ball(window->getWidth(), window->getHeight(), m_player1->getID());
}

Multiplayer::~Multiplayer()
{
    delete m_player1;
    delete m_player2;
    delete m_ball;
}

void Multiplayer::processObjectEvent(SDL_Event &event)
{
    m_player1->processEvent(event, *m_ball);
    m_player2->processEvent(event, *m_ball);
}

void Multiplayer::update(int width, int height)
{
    m_player1->update(width);
    m_player2->update(width);
    m_ball->update(*m_player1, width, height);
    m_ball->update(*m_player2, width, height);
}

void Multiplayer::drawObjects(SDL_Renderer *renderer)
{
    if(m_ball->getOwner() == PLAYER1) {
        m_player2->draw(renderer);
        m_player1->draw(renderer);
    } else {
        m_player1->draw(renderer);
        m_player2->draw(renderer);
    }
    m_ball->draw(renderer);
}

void Multiplayer::drawScore()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Score", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 68, 84, 255));
    ImGui::Text("Player 1 score: %d", m_player1->getScore());
    ImGui::PopStyleColor();
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 193, 226, 255));
    ImGui::Text("Player 2 score: %d", m_player2->getScore());
    ImGui::PopStyleColor();
    ImGui::End();
}
