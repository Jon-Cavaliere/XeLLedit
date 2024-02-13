#pragma once
#include <stdint.h>
#include <SDL.h>

namespace JC
{
	class Color
	{
	public:
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		Color(uint8_t r, uint8_t g, uint8_t b);
		Color(const JC::Color& c);

		/**
		* Set's each color channel's value
		*/
		void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		/**
		* Returns the value for the specified color channel
		*/
		uint8_t& get(uint8_t index);
		const uint8_t read(uint8_t index) const;

		// Sets the specified renderer color to this color
		void setRenderColor(SDL_Renderer* renderer)
		{
			SDL_SetRenderDrawColor(renderer, m_Channels[0], m_Channels[1], m_Channels[2], m_Channels[3]);
		}

		Color operator=(const Color& c);

		uint8_t& operator[](uint8_t index);

		// Public static color objects
		static Color RED;
		static Color BLUE;
		static Color GREEN;
		static Color WHITE;
		static Color BLACK;

		SDL_Color toSDL();

	private:
		uint8_t m_Channels[4]; // R,G,B,A
	};
}


