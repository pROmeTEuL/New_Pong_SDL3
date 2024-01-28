#ifndef MENU_H
#define MENU_H

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>
#include <memory>
#include <functional>
#include <iostream>

#include "window.h"

class Menu
{
public:
    enum class State {
        HIDDEN,
        MAIN_MENU,
        PAUSED,
        MULTIPLAYER,
        JOIN,
        WAITING_FOR_GUEST,
        WAITING_FOR_HOST
    };

public:
    static Menu &instance();
    void draw();
    void setWindow(const std::shared_ptr<Window> &window);
    void setPaused();
    void togglePause();
    void setSinglePlayerCallback(const std::function<void ()> &newSinglePlayerCallback);
    void setMultiPlayerCallback(const std::function<void ()> &newMultiPlayerCallback);
    void setResumeCallback(const std::function<void ()> &newResumeCallback);
    void setQuitCallback(const std::function<void ()> &newQuitCallback);
    void setDrawScore(const std::function<void ()> &newDrawScore);
    void setMainMenuCallback(const std::function<void ()> &newMainMenuCallback);
    void setPausedMenuQuitCallback(const std::function<void ()> &newPausedMenuQuitCallback);
    void setGuestJoined(const std::function<void (SDLNet_StreamSocket*)> &newGuestJoined);
    void setConnectedToHost(const std::function<void (SDLNet_StreamSocket *)> &newConnectedToHost);

    bool isPaused() const;
    State getState() const;
    void setState(State newState);

private:
    Menu() = default;
private:
    std::shared_ptr<Window> m_window;
    std::function<void()> m_singlePlayerCallback;
    std::function<void()> m_multiPlayerCallback;
    std::function<void()> m_resumeCallback;
    std::function<void()> m_quitCallback;
    std::function<void()> m_drawScore;
    std::function<void()> m_mainMenuCallback;
    std::function<void()> m_pausedMenuQuitCallback;
    std::function<void(SDLNet_StreamSocket*)> m_guestJoined;
    std::function<void(SDLNet_StreamSocket*)> m_connectedToHost;
    State m_state = State::MAIN_MENU;
    SDLNet_Server *m_server = nullptr;
    SDLNet_StreamSocket *m_socket = nullptr;
};

#endif // MENU_H
