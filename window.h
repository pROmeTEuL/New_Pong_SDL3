#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <string_view>

class Window
{
public:
    Window(std::string_view title, int width = 640, int height = 480);
    ~Window();
    void close();
    void clear(int r, int g, int b);
    void setColor(int r, int g, int b, int a = 255);
    void draw(SDL_Texture *texture, SDL_FRect *fRect);
    void display();
    bool pollEvent(SDL_Event &event);
    SDL_Window *getWindow() const;
    SDL_Renderer *getRenderer() const;

    int getWidth() const;
    int getHeight() const;
private:
    int m_width, m_height;
    SDL_Window *m_window = NULL;
    SDL_Renderer *m_renderer = NULL;
    int wflags = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;
};

#endif // WINDOW_H
