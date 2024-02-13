#pragma once
#include <string>
#include <stdint.h>
#include <SDL.h>
#include <SDL_image.h>

/* 
    Code directly ripped from lazyfoo's tutorial on sprite sheet clipping with a few minor tweaks
        Thanks! @ https://lazyfoo.net/tutorials/SDL/11_clip_rendering_and_sprite_sheets/index.php
*/

class Texture
{
public:
    //Initializes variables
    Texture();

    //Deallocates memory
    ~Texture();

    //Loads image at specified path
    bool loadFromFile(std::string path, bool colorkey, SDL_Renderer* gRenderer);

    //Deallocates texture
    void free();

    //Renders texture at given point
    void render(int32_t x, int32_t y, SDL_Rect* clip = NULL);

    //Renders texture at given point, at a given size
    void renderSize(int32_t x, int32_t y, int32_t w, int32_t h, SDL_Rect* clip = nullptr);

    //Gets image dimensions
    int getWidth();
    int getHeight();

    // SET THE RENDERER BEFORE LOADING or RENDERING (OBVIOUSLY)
    void setRenderer(SDL_Renderer* renderer);

private:
    // The actual hardware texture
    SDL_Texture* m_Texture;

    // Image dimensions
    uint32_t m_Width;
    uint32_t m_Height;

    // Pointer to the renderer that will be used
    SDL_Renderer* p_Renderer;
};