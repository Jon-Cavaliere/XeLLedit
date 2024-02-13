#pragma once
#include <SDL.h>
#include "Input.h"
#include "Vec2i.h"
#include "Texture.h"

class Button
{
public:
	Button(int32_t x, int32_t y, int32_t w, int32_t h);

	void init(const char* spriteFile, SDL_Renderer* renderer);

	void update(JC::Input* in);

	void render(SDL_Renderer* renderer);

	bool isHover();

	int32_t getX();
	int32_t getY();

	int32_t getW();
	int32_t getH();

private:
	// Screen coordinate position of the button
	JC::Vec2i m_Location;
	// Dimensions of the button in pixels
	JC::Vec2i m_Dimensions;

	// Whether or not the button is currently being hovered by the mouse
	bool m_Hovered;

	// Two frame sprite sheet for idle and hover button
	Texture m_Sheet;
};