#include "menu.h"
#include <iostream>

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
        if (m_chatLog.size() > 0)
            m_chatLog.clear();
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
            m_server = SDLNet_CreateServer(nullptr, 4200);
            m_state = State::WAITING_FOR_GUEST;
        }
        if (ImGui::Button("Join game")) {
            m_state = State::JOIN;
        }
        if (ImGui::Button("Back")) {
            m_state = State::MAIN_MENU;
        }
        ImGui::End();
        break;
    case State::WAITING_FOR_GUEST:
        ImGui::SetNextWindowPos(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 4));
        ImGui::SetNextWindowSize(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 2));
        ImGui::Begin("Hosting game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Waiting for player2 to join %c", "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
        if (ImGui::Button("Back")) {
            m_state = State::MULTIPLAYER;
            SDLNet_DestroyServer(m_server);
            m_server = nullptr;
        }
        ImGui::End();
        if (m_server && !SDLNet_AcceptClient(m_server, &m_socket) && m_socket) {
            if (m_guestJoined) {
                m_guestJoined(m_socket);
                m_socket = nullptr;
            }
            SDLNet_DestroyServer(m_server);
            m_server = nullptr;
            m_state = State::HIDDEN;
        }
        break;
    case State::JOIN: {
        ImGui::SetNextWindowPos(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 4));
        ImGui::SetNextWindowSize(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 2));
        ImGui::Begin("Join game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
        static char ip[251] = "127.0.0.1";
        ImGui::InputText("Ip", ip, 250);
        if (ImGui::Button("Join")) {
            SDLNet_Address *address = SDLNet_ResolveHostname(ip);
            while (SDLNet_GetAddressStatus(address) == 0);
            if (SDLNet_GetAddressStatus(address) != 1) {
                std::cerr << "Can't resolve hostname: " << ip << std::endl;
            } else {
                m_socket = SDLNet_CreateClient(address, 4200);
                m_state = State::WAITING_FOR_HOST;
            }
            SDLNet_UnrefAddress(address);
        }
        if (ImGui::Button("Back")) {
            m_state = State::MULTIPLAYER;
        }
        ImGui::End();
    }
    break;
    case State::WAITING_FOR_HOST:
        ImGui::SetNextWindowPos(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 4));
        ImGui::SetNextWindowSize(ImVec2(m_window->getWidth() / 3, m_window->getHeight() / 2));
        ImGui::Begin("Joining game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Connecting to host %c", "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
        if (ImGui::Button("Back")) {
            m_state = State::JOIN;
            SDLNet_DestroyStreamSocket(m_socket);
            m_socket = nullptr;
        }
        ImGui::End();
        if (m_socket && SDLNet_GetConnectionStatus(m_socket) == 1) {
            if (m_connectedToHost) {
                m_connectedToHost(m_socket);
                m_socket = nullptr;
            }
            m_state = State::HIDDEN;
        }
        break;
    case State::CHAT:
        ImGui::SetNextWindowPos(ImVec2(m_window->getWidth() / 4, 0));
        ImGui::SetNextWindowSize(ImVec2(m_window->getWidth() / 2, m_window->getHeight() / 2));
        ImGui::Begin("Chat", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
        for (const auto &el : m_chatLog) {
            ImGui::Text("%s", el.c_str());
        }
        if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
            ImGui::SetKeyboardFocusHere(0);
        static char msg[251] = {0};
        if (ImGui::InputText(" ", msg, 250, ImGuiInputTextFlags_EnterReturnsTrue) && msg[0] != 0) {
            if (m_sendMessage)
                m_sendMessage(msg);
            m_chatLog.push_back(msg);
            msg[0] = 0;
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
    return m_state == State::PAUSED || m_state == State::CHAT;
}

Menu::State Menu::getState() const
{
    return m_state;
}

void Menu::setState(State newState)
{
    m_state = newState;
}

void Menu::addChat(const std::string &msg)
{
    m_chatLog.push_back("* " + msg);
}

void Menu::setSendMessage(const std::function<void (const std::string &)> &newSendMessage)
{
    m_sendMessage = newSendMessage;
}

void Menu::setGuestJoined(const std::function<void (SDLNet_StreamSocket *)> &newGuestJoined)
{
    m_guestJoined = newGuestJoined;
}

void Menu::setConnectedToHost(const std::function<void (SDLNet_StreamSocket*)> &newConnectedToHost)
{
    m_connectedToHost = newConnectedToHost;
}

void Menu::setPausedMenuQuitCallback(const std::function<void ()> &newPausedMenuQuitCallback)
{
    m_pausedMenuQuitCallback = newPausedMenuQuitCallback;
}
