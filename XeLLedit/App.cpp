#include "App.h"

// Version 0.2a adds a character brush selection panel
// Version 0.3a adds grid resize keys
	// CNTRL + Arrow Key - Increase grid size by one row/column in the direction of the arrow
	// CNTRL + SHIFT + Arrow Key - Decrease grid size by one row/column in the direction of the arrow
// Version 0.4a adds selection panel tools and buttons



// It appears that the maximum width for the original XeLL Reloaded art is 74 characters
App::App() : m_Running(true), m_Window(1280, 720, "XeLLedit by Codeus v0.3a"), m_Grid(74, 20), p_RateRegulator(nullptr)
{
	p_RateRegulator = JC::RateRegulator::getInstance();
}

void App::run()
{
	init();
	while (m_Running)
	{
		update();
		render();
	}
	cleanup();
}

void App::init()
{
	m_Window.init();
	m_Grid.init(m_Window.getRenderer());
	m_Grid.setWindow(m_Window.getWindow());
}

void App::update()
{
	m_Input.update();
	if (m_Input.shouldQuit())
		m_Running = false;
	
	m_Grid.update(&m_Input);
}

void App::render()
{
	p_RateRegulator->begin();
	SDL_SetRenderDrawColor(m_Window.getRenderer(), 0x00, 0x00, 0xFF, 0x00);
	SDL_RenderClear(m_Window.getRenderer());

	// Render stuff here
	m_Grid.render(m_Window.getRenderer());
	

	SDL_RenderPresent(m_Window.getRenderer());
	p_RateRegulator->end();
}

void App::cleanup()
{
	m_Window.clean();
}
