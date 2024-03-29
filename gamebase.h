#ifndef GAMEBASE_H
#define GAMEBASE_H

#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>
#include <memory>

#include "window.h"

class GameBase
{
public:
    GameBase(const std::shared_ptr<Window> &window);
    virtual ~GameBase();
    void run();
protected:
    virtual void processEvent();
    virtual void processObjectEvent(SDL_Event &event) = 0;
    virtual void update(int width, int height) = 0;
    bool draw();
    virtual void drawObjects(SDL_Renderer *renderer) = 0;
protected:
    std::shared_ptr<Window> m_window;
    bool m_playing = true;
    bool m_paused = false;
};

#endif // GAMEBASE_H
