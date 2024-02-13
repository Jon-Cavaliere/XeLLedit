#pragma once
#include "Color.h"
#include "Vec2i.h"
#include <SDL.h>


namespace JC
{
	class Rect
	{
	public:
		Rect(int32_t x, int32_t y, int32_t w, int32_t h);
		Rect(int32_t x, int32_t y, int32_t w, int32_t h, JC::Color color);

		void set(int32_t x, int32_t y, int32_t w, int32_t h);
		void set(int32_t x, int32_t y);
		void setSize(int32_t w, int32_t h);
		void setColor(JC::Color color);

		// Get rectangle components
		JC::Vec2i& getPos();
		JC::Vec2i& getSize();

		// Gets the center of the rectangle
		int32_t getCenterX();
		int32_t getCenterY();

		int32_t getX();
		int32_t getY();

		void setX(int32_t x);
		void setY(int32_t y);

		void getCenter(Vec2i& center);

		SDL_Rect toSDLRect();

		void Render(SDL_Renderer* renderer);

		static bool checkCollision(Rect& a, Rect& b);

	private:
		JC::Vec2i m_Pos;
		JC::Vec2i m_Size;
		JC::Color m_Color;
	};
}