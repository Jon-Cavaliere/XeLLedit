#include "Window.h"

JC::Window::Window()
    : Window(1080, 720, "Default Title")
{ }

JC::Window::Window(uint16_t width, uint16_t height, const char* title)
	: m_Width(width), m_Height(height), m_Title(title), m_Resized(false)
{ }

bool JC::Window::init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		printf("SDL2 initialized successfully!\n");

		// ----- Set up Window -----

		//Create window
		m_Window = SDL_CreateWindow(m_Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_Width, m_Height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
		if (m_Window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}


		// ----- Set up Renderer -----
		p_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

		if (p_Renderer == NULL)
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Initialize renderer color
			printf("Renderer initialized! \n");
			SDL_SetRenderDrawColor(p_Renderer, 0, 0, 0, 0);
		}

	}
	else
	{
		printf("SDL2 failed to initialize, %s\n", SDL_GetError());
		success = false;
	}

	// Init SDL_Image

	if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}
	else
	{
		printf("SDL_image initialized!\n");
	}
	SDL_Surface* icon = IMG_Load("res/XEIcon.png");
	SDL_SetWindowIcon(m_Window, icon);

	// Set Render Size
	// SDL_RenderSetLogicalSize(p_Renderer, 640, 480); 

	return success;
}

void JC::Window::clean()
{
	//Destroy window	
	SDL_DestroyRenderer(p_Renderer);
	SDL_DestroyWindow(m_Window);
	m_Window = NULL;
	p_Renderer = NULL;

	//Quit SDL
	SDL_Quit();
}

void JC::Window::resize(uint16_t width, uint16_t height)
{
	m_Width = width;
	m_Height = height;
	m_Resized = true;
}


SDL_Renderer* JC::Window::getRenderer()
{
	return p_Renderer;
}

SDL_Window* JC::Window::getWindow()
{
	return m_Window;
}

uint16_t JC::Window::getWidth()
{
	return m_Width;
}

uint16_t JC::Window::getHeight()
{
	return m_Height;
}
