#include "menu.h"

Menu &Menu::instance()
{
    static Menu it;
    return it;
}

void Menu::draw()
{
    if (m_state == State::HIDDEN && !m_drawScore)
        return;
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    if (m_drawScore)
        m_drawScore();
    switch (m_state) {
    case State::MAIN_MENU:
        ImGui::SetNextWindowPos(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 4));
        ImGui::SetNextWindowSize(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 2));
        ImGui::Begin("Main menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
        if (ImGui::Button("Singleplayer")) {
            if (m_singlePlayerCallback)
                m_singlePlayerCallback();
            m_state = State::HIDDEN;
        }
        if (ImGui::Button("Multiplayer")) {
//            if (m_multiPlayerCallback)
//                m_multiPlayerCallback();
            m_state = State::MULTIPLAYER;
        }
        if (ImGui::Button("Quit")) {
            if (m_quitCallback)
                m_quitCallback();
            m_state = State::HIDDEN;
        }
        ImGui::End();
        break;
    case State::PAUSED:
        ImGui::SetNextWindowPos(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 4));
        ImGui::SetNextWindowSize(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 2));
        ImGui::Begin("Pause", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
        if (ImGui::Button("Resume")) {
            if (m_resumeCallback)
                m_resumeCallback();
            m_state = State::HIDDEN;
        }
        if (ImGui::Button("Main Menu")) {
            if (m_mainMenuCallback)
                m_mainMenuCallback();
            m_state = State::MAIN_MENU;
        }
        if (ImGui::Button("Quit")) {
            if (m_pausedMenuQuitCallback)
                m_pausedMenuQuitCallback();
            if (m_quitCallback)
                m_quitCallback();
            m_state = State::HIDDEN;
        }
        ImGui::End();
        break;
    case State::MULTIPLAYER:
        ImGui::SetNextWindowPos(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 4));
        ImGui::SetNextWindowSize(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 2));
        ImGui::Begin("Multiplayer menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
        if (ImGui::Button("Local play")) {
            if (m_multiPlayerCallback)
                m_multiPlayerCallback();
            m_state = State::HIDDEN;
        }
        if (ImGui::Button("Host game")) {
            m_state = State::HOST;
        }
        if (ImGui::Button("Join game")) {
            m_state = State::JOIN;
        }
        if (ImGui::Button("Back")) {
            m_state = State::MAIN_MENU;
        }
        ImGui::End();
        break;
    case State::HOST:
        ImGui::SetNextWindowPos(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 4));
        ImGui::SetNextWindowSize(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 2));
        ImGui::Begin("Host game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
        if (ImGui::Button("Back")) {
            m_state = State::MULTIPLAYER;
        }
        ImGui::End();
        break;
    case State::JOIN:
        ImGui::SetNextWindowPos(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 4));
        ImGui::SetNextWindowSize(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 2));
        ImGui::Begin("Join game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
        //ImGui::InputText("Ip");
        if (ImGui::Button("Back")) {
            m_state = State::MULTIPLAYER;
        }
        ImGui::End();
        break;
    default:
        break;
    }
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
}

void Menu::setWindow(const std::shared_ptr<Window> &window)
{
    m_window = window;
}

void Menu::setPaused()
{
    m_state = State::PAUSED;
}

void Menu::togglePause()
{
    if (m_state == State::HIDDEN)
        m_state = State::PAUSED;
    else
        m_state = State::HIDDEN;
}

void Menu::setSinglePlayerCallback(const std::function<void ()> &newSinglePlayerCallback)
{
    m_singlePlayerCallback = newSinglePlayerCallback;
}

void Menu::setMultiPlayerCallback(const std::function<void ()> &newMultiPlayerCallback)
{
    m_multiPlayerCallback = newMultiPlayerCallback;
}

void Menu::setResumeCallback(const std::function<void ()> &newResumeCallback)
{
    m_resumeCallback = newResumeCallback;
}

void Menu::setQuitCallback(const std::function<void ()> &newQuitCallback)
{
    m_quitCallback = newQuitCallback;
}

void Menu::setDrawScore(const std::function<void ()> &newDrawScore)
{
    m_drawScore = newDrawScore;
}

void Menu::setMainMenuCallback(const std::function<void ()> &newMainMenuCallback)
{
    m_mainMenuCallback = newMainMenuCallback;
}

bool Menu::isPaused() const
{
    return m_state == State::PAUSED;
}

void Menu::setPausedMenuQuitCallback(const std::function<void ()> &newPausedMenuQuitCallback)
{
    m_pausedMenuQuitCallback = newPausedMenuQuitCallback;
}
