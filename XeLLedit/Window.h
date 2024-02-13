#pragma once
#include <stdint.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
namespace JC
{
	class Window
	{
	public:
		Window();
		Window(uint16_t width, uint16_t height, const char* title);

		bool init();
		void clean();

		SDL_Renderer* getRenderer();
		SDL_Window* getWindow();

		uint16_t getWidth();
		uint16_t getHeight();

	private:
		uint16_t m_Width;
		uint16_t m_Height;
		std::string m_Title;
		SDL_Window* m_Window = NULL;
		SDL_Renderer* p_Renderer = NULL;
	};

}