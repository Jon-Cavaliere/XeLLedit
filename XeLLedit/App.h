#pragma once
#include "Input.h"
#include "Window.h"
#include "CharGrid.h"
#include "RateRegulator.h"
class App
{
public:
	App();
	void run();
private:
	void init();
	void update();
	void render();
	void cleanup();

	bool m_Running;
	JC::Input m_Input;
	JC::Window m_Window;
	JC::RateRegulator* p_RateRegulator;

	// Grid of Characters
	CharGrid m_Grid;
};