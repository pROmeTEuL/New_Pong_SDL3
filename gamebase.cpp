#include "gamebase.h"
#include <thread>
#include "menu.h"

GameBase::GameBase(const std::shared_ptr<Window> &window)
    : m_window(window)
{
    Menu::instance().setMainMenuCallback([this]{
        m_playing = false;
    });
    Menu::instance().setPausedMenuQuitCallback([this]{
        m_playing = false;
    });
}

GameBase::~GameBase()
{
    Menu::instance().setDrawScore({});
}

void GameBase::run()
{
    while (m_playing) {
        processEvent();
        if (!Menu::instance().isPaused())
            update(m_window->getWidth(), m_window->getHeight());  /// virt
        draw();
        std::this_thread::sleep_for(std::chrono::microseconds{1000});
    }
}

void GameBase::processEvent()
{
    SDL_Event event;
    while (m_window->pollEvent(event)) {
        processObjectEvent(event); /// virt
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.keysym.sym == SDLK_ESCAPE)
            Menu::instance().togglePause();
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.keysym.sym == SDLK_BACKQUOTE) {
            Menu::instance().setState(Menu::State::CHAT);
        }
    }
}

bool GameBase::draw()
{

    m_window->clear(0, 0, 0);
    Menu::instance().draw();
    drawObjects(m_window->getRenderer());  /// virt
    if (m_paused) {
    }
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
    m_window->display();
    return false;
}
