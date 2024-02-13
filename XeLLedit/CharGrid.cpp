#include "CharGrid.h"
#include "Color.h"
#include <iostream>
#include <fstream>
#include <SDL_syswm.h>
#include <commdlg.h>
#include <Windows.h>
#include <codecvt>
#include <locale>

CharGrid::CharGrid(uint16_t w, uint16_t h, uint16_t winWidth, uint16_t winHeight) :
	m_Width(w), m_Height(h), m_CharSheet(), m_CurChar(1), m_Initialized(false), m_Cursor(0, 0), m_TileSize(10), m_StartCoords(0, 0), m_ShowGrid(true),
	m_MoveSpeed(1), m_WinWidth(winWidth), m_WinHeight(winHeight), m_TileSizePanel(20), m_HoverActive(-1), p_MainWindow(nullptr), m_ActiveTool(Tool::LINE),
	m_MouseDrag(false), m_DragStart(0, 0), m_DrawButton(730, 540, 100, 50), m_LineButton(730, 600, 100, 50), m_RectButton(730, 660, 100, 50), m_Arrow()
{

	// Default preview tile size is 2x actual tilesize
	m_TileSizePanel = 2 * m_TileSize;
	for (int x = 0; x < w; x++)
	{
		std::vector<uint8_t> column;
		for (int y = 0; y < h; y++)
		{
			column.push_back(32);
		}
		m_Tiles.push_back(column);
	}

	// Calculate a starting location that is centered horizontally, and push it down a bit vertically
	// Get total size of grid in pixels, accounting for 2px spacers
	uint16_t widthInPx = m_Width * m_TileSize + (m_Width + 1) * 2; // Width of character grid in pixels including the 2px spacers
	uint16_t difWidth = winWidth - widthInPx;
	m_StartCoords[0] = difWidth / 2;
	m_StartCoords[1] = 40;
}


CharGrid::~CharGrid()
{

}

void CharGrid::init(SDL_Renderer* renderer)
{
	m_CharSheet.loadFromFile("res/cp437_10x10.png", true, renderer);
	SDL_ShowCursor(SDL_DISABLE);

	m_DrawButton.init("res/draw_button.png", renderer);
	m_LineButton.init("res/line_button.png", renderer);
	m_RectButton.init("res/rect_button.png", renderer);

	m_Arrow.loadFromFile("res/select_arrow.png", false, renderer);
}

void CharGrid::update(JC::Input* input)
{
	
	// Update currently selected tile with mouse wheel (EXCLUDE NULL CHARACTER /0 at char val... 0)
	if (input->getScrolling() == 1) // Scrolling up
	{
		if (m_CurChar < 255)
			m_CurChar++;
		else
			m_CurChar = 1;
	}
	else if (input->getScrolling() == -1) // Scrolling down
	{
		// If control is down, change tool
		if (m_CurChar > 1)
			m_CurChar--;
		else
			m_CurChar = 255;
	}

	// Translate the whole grid around, why? Because it's a nice feature
	// Shift + Arrow Key moves it faster
	// L Cntrl + Arrow Key Increases grid size by one in direction of arrow
	if (input->isKeyDown(SDLK_LSHIFT))
		m_MoveSpeed = 3;
	else
		m_MoveSpeed = 1;

	if (input->isKeyDown(SDLK_UP))
	{
		if(!input->isKeyDown(SDLK_LCTRL))
			m_StartCoords[1] += m_MoveSpeed;
		else
		{
			uint32_t downTime = input->getKeyDownTime(SDLK_UP);
			if (downTime <= 1)
			{
				if (input->isKeyDown(SDLK_LSHIFT))
				{
					decreaseGridSize(0);
					m_StartCoords[1] += m_TileSize;
				}
				else
				{
					increaseGridSize(0);
					m_StartCoords[1] -= m_TileSize;
				}
			}
		}
	}
	else if (input->isKeyDown(SDLK_DOWN))
	{
		if (!input->isKeyDown(SDLK_LCTRL))
			m_StartCoords[1] -= m_MoveSpeed;
		else
		{
			uint32_t downTime = input->getKeyDownTime(SDLK_DOWN);
			if (downTime <= 1)
			{
				if (input->isKeyDown(SDLK_LSHIFT))
					decreaseGridSize(1);
				else
				{
					increaseGridSize(1);
				}
			}
		}
	}
	if (input->isKeyDown(SDLK_LEFT))
	{
		if (!input->isKeyDown(SDLK_LCTRL))
			m_StartCoords[0] += m_MoveSpeed;
		else
		{
			uint32_t downTime = input->getKeyDownTime(SDLK_LEFT);
			if (downTime <= 1)
			{
				if (input->isKeyDown(SDLK_LSHIFT))
				{
					decreaseGridSize(2);
					m_StartCoords[0] += m_TileSize;
				}
				else
				{
					increaseGridSize(2);
					m_StartCoords[0] -= m_TileSize;
				}
			}
		}
	}
	else if (input->isKeyDown(SDLK_RIGHT))
	{
		if (!input->isKeyDown(SDLK_LCTRL))
			m_StartCoords[0] -= m_MoveSpeed;
		else
		{
			uint32_t downTime = input->getKeyDownTime(SDLK_RIGHT);
			if (downTime <= 1)
			{
				if(input->isKeyDown(SDLK_LSHIFT))
					decreaseGridSize(3);
				else
					increaseGridSize(3);
			}
		}
	}

	// Cntrl + C Clear all tiles ( Sets the to space )
	if ((input->isKeyDown(SDLK_LCTRL) || input->isKeyDown(SDLK_RCTRL)) && input->isKeyDown(SDL_SCANCODE_C))
		clear();

	// Cntrl + A set all tiles to active character
	if ((input->isKeyDown(SDLK_LCTRL) || input->isKeyDown(SDLK_RCTRL)) && input->isKeyDown(SDL_SCANCODE_A))
		setAll(m_CurChar);

	// Save?
	if ((input->isKeyDown(SDLK_LCTRL) || input->isKeyDown(SDLK_RCTRL)) && input->isKeyDown(SDL_SCANCODE_S))
	{
		// I'll probably put in a cooldown timer for save so that it doesn't try to save a million times while keys are down
		uint32_t downTime = input->getKeyDownTime(SDL_SCANCODE_S);
		if (downTime <= 1)
		{
			printf("Save\n");
			save("output\\asciiart.h");
		}
	}

	// Load
	if ((input->isKeyDown(SDLK_LCTRL) || input->isKeyDown(SDLK_RCTRL)) && input->isKeyDown(SDL_SCANCODE_L))
	{
		// I'll probably put in a cooldown timer for save so that it doesn't try to save a million times while keys are down
		uint32_t downTime = input->getKeyDownTime(SDL_SCANCODE_L);
		if (downTime <= 1)
		{
			printf("Load:\n");
			load("Nope");
		}
	}
	
	// Update mouse position by copying values... #TODO Fix this
	m_Cursor[0] = input->getCursorPos()[0];
	m_Cursor[1] = input->getCursorPos()[1];

	// If the mouse is over the selection panel, show cursor
	if (m_Cursor[1] >= m_WinHeight - 200)
	{
		SDL_ShowCursor(SDL_ENABLE);
		
		// Update buttons
		m_DrawButton.update(input);
		m_LineButton.update(input);
		m_RectButton.update(input);

		// Select tile
		if (input->isMouseDown(0))
		{
			if(m_HoverActive > -1)
				m_CurChar = m_HoverActive;
			else
			{
				if (m_DrawButton.isHover())
					m_ActiveTool = Tool::DRAW;
				else if (m_LineButton.isHover())
					m_ActiveTool = Tool::LINE;
				else if (m_RectButton.isHover())
					m_ActiveTool = Tool::RECT;
			}

		}
	}
	// If mouse is over the char grid edit area
	else
	{
		// Right click selects the tile clicked and sets m_curChar to dat (Tool independent)
		if (input->isMouseDown(2))
		{
			m_CurChar = tileAtMouse(m_Cursor[0], m_Cursor[1]);
		}

		// If tool is draw
		if (m_ActiveTool == Tool::DRAW)
		{
			SDL_ShowCursor(SDL_DISABLE);

			// Left click places tile
			if (input->isMouseDown(0))
			{
				tileAtMouse(m_Cursor[0], m_Cursor[1]) = m_CurChar;
			}
		}
		// If the tool is line (Only vertical and horizontal, no diagonal)
		else if (m_ActiveTool == Tool::LINE)
		{
			// Left click places tile
			if (input->isMouseDown(0))
			{
				if (!m_MouseDrag)
				{
					m_MouseDrag = true;
					m_DragStart.set(m_Cursor[0], m_Cursor[1]);
				}
			}
			else
			{
				// Check if mouse was being dragged
				if (m_MouseDrag)
				{
					m_MouseDrag = false;
					// Add line from m_DragStart to m_Cursor
					addLine();
				}
			}
		}
		// If the tool is rect
		else if (m_ActiveTool == Tool::RECT)
		{
			// Left click places tile
			if (input->isMouseDown(0))
			{
				if (!m_MouseDrag)
				{
					m_MouseDrag = true;
					m_DragStart.set(m_Cursor[0], m_Cursor[1]);
				}
			}
			else
			{
				// Check if mouse was being dragged
				if (m_MouseDrag)
				{
					m_MouseDrag = false;
					// Add rect from m_DragStart to m_Cursor
					addRect();
				}
			}
		}
	}
}

void CharGrid::render(SDL_Renderer* renderer)
{
	// Render grid
	SDL_SetRenderDrawColor(renderer, 115, 115, 115, 50);

	// Each grid line is going to be 2 pixels in stroke

	// Draw vertical grid lines
	uint16_t widthInPx = m_Width * m_TileSize + (m_Width + 1) * 2; // Width of character grid in pixels including the 2px spacers
	uint16_t heightInPx = m_Height * m_TileSize + (m_Height + 1) * 2; // Height of character grid in pixels including the 2px spacers
	uint8_t stepSizePx = m_TileSize + 2;
	for (int x = m_StartCoords[0]; x < widthInPx + m_StartCoords[0]; x += stepSizePx)
	{
		// Draw a rect 
		SDL_Rect line = { x, m_StartCoords[1], 2, heightInPx };
		SDL_RenderFillRect(renderer, &line);
	}

	// Draw Horizontal grid lines

	for (int y = m_StartCoords[1]; y < heightInPx + m_StartCoords[1]; y += stepSizePx)
	{
		// Draw a rect 
		SDL_Rect line = { m_StartCoords[0], y, widthInPx, 2 };
		SDL_RenderFillRect(renderer, &line);
	}

	// Draw all of the tiles in the tilegrid array
	int16_t renderX = m_StartCoords[0] + 2;
	int16_t renderY = m_StartCoords[1] + 2;
	for (uint16_t x = 0; x < m_Tiles.size(); x++)
	{
		for (uint16_t y = 0; y < m_Tiles[x].size(); y++)
		{
			// Render current tile
			renderX = m_StartCoords[0] + 2 + x * 12;
			renderY = m_StartCoords[1] + 2 + y * 12;

			SDL_Rect clip = IdToClip(m_Tiles[x][y]);
			m_CharSheet.render(renderX, renderY, &clip);
		}
	}

	// Draw currently selected tile at cursor if it's a character. If it's a space, draw a box
	if (m_CurChar != 32)
	{
		SDL_Rect clip = IdToClip();
		m_CharSheet.render(m_Cursor[0] - m_TileSize / 2, m_Cursor[1] - m_TileSize / 2, &clip);
	}
	else
	{
		SDL_Rect box = { m_Cursor[0] - m_TileSize / 2, m_Cursor[1] - m_TileSize / 2, 10, 10 };
		SDL_RenderDrawRect(renderer, &box);
	}

	// Draw preview for line
	if (m_ActiveTool == Tool::LINE)
	{
		if (m_MouseDrag)
		{
			// Calculate exact overlay coordinates so that we may draw preview of characters
			int rX = 0, rY = 0;

			// Vertical or horizontal
			int dstX = m_Cursor[0] - m_DragStart[0];
			int dstY = m_Cursor[1] - m_DragStart[1];

			uint32_t tileX = 0;
			uint32_t tileY = 0;

			// Horizontal Line
			if (std::abs(dstX) > std::abs(dstY))
			{
				//tileAtMouse(m_Cursor[0], m_Cursor[1]) = m_CurChar;
				// If m_Cursor x was greater than m_DragStart x, start at m_DragStart
				if (dstX > 0)
				{
					if (m_DragStart[0] <= m_StartCoords[0] + 2)
						m_DragStart[0] = m_StartCoords[0] + 2;
					tileCoordAtMouse(m_DragStart[0], m_DragStart[1], tileX, tileY);
					for (int x = tileX; x < m_Cursor[0]; x += m_TileSize + 2)
					{
						SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
						SDL_Rect box = { x, tileY, 10, 10 };
						SDL_RenderFillRect(renderer, &box);
						if (m_CurChar != 32)
						{
							SDL_Rect clip = IdToClip();
							m_CharSheet.render(x, tileY, &clip);
						}
						else
						{
							SDL_SetRenderDrawColor(renderer, 115, 115, 115, 50);
							SDL_Rect box = {x, tileY, 10, 10 };
							SDL_RenderDrawRect(renderer, &box);
						}
					}
				}
				// Otherwise start at cursor
				else
				{
					if (m_Cursor[0] <= m_StartCoords[0] + 2)
						m_Cursor[0] = m_StartCoords[0] + 2;
					tileCoordAtMouse(m_Cursor[0], m_DragStart[1], tileX, tileY);
					for (int x = tileX; x < m_DragStart[0]; x += m_TileSize + 2)
					{
						SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
						SDL_Rect box = { x, tileY, 10, 10 };
						SDL_RenderFillRect(renderer, &box);
						if (m_CurChar != 32)
						{
							SDL_Rect clip = IdToClip();
							m_CharSheet.render(x, tileY, &clip);
						}
						else
						{
							SDL_SetRenderDrawColor(renderer, 115, 115, 115, 50);
							SDL_Rect box = { x, tileY, 10, 10 };
							SDL_RenderDrawRect(renderer, &box);
						}
					}
				}
			}
			// Vertical line
			else
			{
				// If m_Cursor y was greater than m_DragStart y, start at m_DragStart
				if (dstY > 0)
				{
					if (m_DragStart[1] <= m_StartCoords[1] + 2)
						m_DragStart[1] = m_StartCoords[1] + 2;
					tileCoordAtMouse(m_DragStart[0], m_DragStart[1], tileX, tileY);

					for (int y = tileY; y < m_Cursor[1]; y += m_TileSize + 2)
					{
						SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
						SDL_Rect box = { tileX, y, 10, 10 };
						SDL_RenderFillRect(renderer, &box);
						if (m_CurChar != 32)
						{
							SDL_Rect clip = IdToClip();
							m_CharSheet.render(tileX, y, &clip);
						}
						else
						{
							SDL_SetRenderDrawColor(renderer, 115, 115, 115, 50);
							SDL_Rect box = {tileX, y, 10, 10 };
							SDL_RenderDrawRect(renderer, &box);
						}
					}
				}
				// Otherwise start at cursor
				else
				{
					if (m_Cursor[1] <= m_StartCoords[1] + 2)
						m_Cursor[1] = m_StartCoords[1] + 2;
					tileCoordAtMouse(m_DragStart[0], m_Cursor[1], tileX, tileY);
					for (int y = tileY; y < m_DragStart[1]; y += m_TileSize + 2)
					{
						SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
						SDL_Rect box = { tileX, y, 10, 10 };
						SDL_RenderFillRect(renderer, &box);
						if (m_CurChar != 32)
						{
							SDL_Rect clip = IdToClip();
							m_CharSheet.render(tileX, y, &clip);
						}
						else
						{
							SDL_SetRenderDrawColor(renderer, 115, 115, 115, 50);
							SDL_Rect box = { tileX, y, 10, 10 };
							SDL_RenderDrawRect(renderer, &box);
						}
					}
				}
			}
		}
	}
	// Draw a preview for rect
	else if (m_ActiveTool == Tool::RECT)
	{
		if (m_MouseDrag)
		{
			int dstX = m_Cursor[0] - m_DragStart[0];
			int dstY = m_Cursor[1] - m_DragStart[1];

			int startX = 0;
			int startY = 0;
			int endX = 0;
			int endY = 0;

			// Horizontal starting and ending point
			// If m_Cursor x was greater than m_DragStart x, start at m_DragStart
			if (dstX > 0)
			{
				startX = m_DragStart[0];
				endX = m_Cursor[0];
			}
			// Otherwise start at cursor
			else
			{
				startX = m_Cursor[0];
				endX = m_DragStart[0];
			}

			// Vertical starting and ending point
			// If m_Cursor y was greater than m_DragStart y, start at m_DragStart
			if (dstY > 0)
			{
				startY = m_DragStart[1];
				endY = m_Cursor[1];
			}
			// Otherwise start at cursor
			else
			{
				startY = m_Cursor[1];
				endY = m_DragStart[1];
			}

			uint32_t tileStartX = 0;
			uint32_t tileStartY = 0;

			if (startX <= m_StartCoords[0] + 2)
				startX = m_StartCoords[0] + 2;
			if (startY <= m_StartCoords[1] + 2)
				startY = m_StartCoords[1] + 2;

			tileCoordAtMouse(startX, startY, tileStartX, tileStartY);


			for (int x = tileStartX; x < endX; x += m_TileSize + 2)
			{
				for (int y = tileStartY; y < endY; y += m_TileSize + 2)
				{
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
					SDL_Rect box = { x, y, 10, 10 };
					SDL_RenderFillRect(renderer, &box);
					if (m_CurChar != 32)
					{
						
						SDL_Rect clip = IdToClip();
						m_CharSheet.render(x, y, &clip);
					}
					else
					{
						SDL_SetRenderDrawColor(renderer, 115, 115, 115, 50);
						SDL_Rect box = { x, y, 10, 10 };
						SDL_RenderDrawRect(renderer, &box);
					}
				}
			}
		}
	}
	// Draw Selection panel
	renderPanel(renderer);
}

std::vector<std::vector<uint8_t>>& CharGrid::getTiles()
{
	return m_Tiles;
}

void CharGrid::save(std::string filename)
{
	// basic file operations
	using namespace std;
	ofstream myfile;
	// Get path to current executable directory
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	std::wstring wFilename = std::wstring(buffer).substr(0, pos).append(L"\\").append(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(filename));
	std::wcout << wFilename << std::endl;
	myfile.open(wFilename);
	if (myfile.is_open())
	{
		std::string head = "const char* asciiart = \"\\n\"";
		myfile << head;
		myfile << '\n';
		for (int i = 0; i < m_Tiles[0].size(); i++)
		{
			myfile << "\t\"";
			for (int j = 0; j < m_Tiles.size(); j++)
			{
				myfile << m_Tiles[j][i];
			}
			if (i < m_Tiles[0].size() - 1)
				myfile << "\\n\"";
			else
				myfile << "\\n\\n\";";

			myfile << '\n';
		}
		myfile << "const char* asciitail =\n\"              Free60.org XeLL RELOADED - Xenon Linux Loader\\n\\n\";";
		myfile.close();
		if (!myfile.is_open())
			printf("File closed.\n");
	}
	else
	{
		std::wcout << L"Error could not open file: " << wFilename << std::endl;
	}
}

/*
	#TODO: This ugly load function is aggressively broken 

	It'll somehow lock down whatever file it loads so that it can't be opened for saving
	I think it's because of the windows API. Because regular filestreams work just fine.

*/
void CharGrid::load(std::string filename)
{
	// Get HWND from SDL
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(p_MainWindow, &wmInfo);
	HWND hwnd = wmInfo.info.win.window;

	// Microsoft open file dialog example code, ripped for use here. Goodbye cross-platform compatibility 
	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[260];    // buffer for file name
	HANDLE hf = nullptr;    // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"/";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
	{
		hf = CreateFile(ofn.lpstrFile,
			GENERIC_ALL,
			FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			(HANDLE)NULL);
	}

	if (hf)
	{
		// Close the handle, so I can open the file with ifstream...
		
		CloseHandle(hf);
		printf("File Closed\n");
	}

	std::string fileContents;
	
	using namespace std;
	string line;
	ifstream myfile(szFile);
	std::vector<std::string> lines;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			cout << line << '\n';
			fileContents.append(line);
			fileContents.append("\n");

			lines.push_back(line);
			 
		}
		myfile.close();
		
		// Remove the first line and the last two lines
		lines.erase(lines.begin());
		lines.erase(lines.end() - 1);
		lines.erase(lines.end() - 1);


		// Loop through each line in the file
		std::cout << "\n";
		std::cout << "\n";
		int lineIndex = 0;
		int maxSize = 0;
		for (auto& l : lines)
		{
			if (lineIndex < lines.size() - 1)
			{
				l = l.substr(2, l.length() - 5);
			}
			else
			{
				l = l.substr(2, l.length() - 8);
			}

			if (l.length() > maxSize)
				maxSize = l.length();

			lineIndex++;
			std::cout << l << "\n";
		}

		m_Tiles.clear();

		m_Width = maxSize;
		m_Height = lines.size();
		for (int x = 0; x < m_Width; x++)
		{
			std::vector<uint8_t> col;
			for (int y = 0; y < m_Height; y++)
			{
				if (x < lines[y].size())
				{
					col.push_back(lines[y][x]);
				}
				else
				{
					col.push_back(32);
				}
			}
			m_Tiles.push_back(col);
		}
	}
	if (!myfile.is_open())
		printf("File closed.\n");
}

void CharGrid::setWindow(SDL_Window* window)
{
	p_MainWindow = window;
}

SDL_Rect CharGrid::IdToClip(uint8_t id)
{
	uint8_t x = id % 16;
	uint8_t y = id / 16;
	SDL_Rect clip = { x * m_TileSize, y * m_TileSize, m_TileSize, m_TileSize };
	return clip;
}

SDL_Rect CharGrid::IdToClip()
{
	return IdToClip(m_CurChar);
}

uint8_t& CharGrid::tileAtMouse(uint32_t x, uint32_t y)
{
	// Cheeky bit of algebra converts render coordinates to tile array coords
	int tileX = (x - 2 - m_StartCoords[0]) / (m_TileSize + 2);
	int tileY = (y - 2 - m_StartCoords[1]) / (m_TileSize + 2);

	if (m_Tiles.size() > tileX && m_Tiles[tileX].size() > tileY)
		return m_Tiles[tileX][tileY];

	// Return space char val with no tile reference if tile does not exist
	uint8_t garbageCatch = 32;
	return garbageCatch;
}

void CharGrid::tileCoordAtMouse(uint32_t mouseX, uint32_t mouseY, uint32_t& tileX, uint32_t& tileY)
{
	// Cheeky bit of algebra converts render coordinates to tile array coords
	tileX = (mouseX - 2 - m_StartCoords[0]) / (m_TileSize + 2) * (m_TileSize + 2) + m_StartCoords[0] + 2;
	tileY = (mouseY - 2 - m_StartCoords[1]) / (m_TileSize + 2) * (m_TileSize + 2) + m_StartCoords[1] + 2;
}

void CharGrid::clear()
{
	setAll(32); // Space
}

void CharGrid::setAll(uint8_t id)
{
	for (auto& column : m_Tiles)
	{
		for (auto& val : column)
		{
			val = id; 
		}
	}
}

void CharGrid::renderPanel(SDL_Renderer* renderer)
{
	// Light Panel
	SDL_SetRenderDrawColor(renderer, 115, 115, 115, 255);

	// Draw Panel at bottom
	SDL_Rect pan{ 0, m_WinHeight - 194, m_WinWidth, 194 };
	SDL_RenderFillRect(renderer, &pan);

	// Darker gray under tile previews
	SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);

	// Draw Panel at bottom
	SDL_Rect subpan{ 5, m_WinHeight - 194 + 5, 712, 194 - 10};
	SDL_RenderFillRect(renderer, &subpan);

	// Draw a bunch of black boxes that will be the character buttons
	// White Panel
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	int x = 10;
	int y = pan.y + 10;
	int row = 0;
	bool hover = false;
	for (int i = 1; i <= 255; i++)
	{
		SDL_Rect box{ x, y, m_TileSizePanel, m_TileSizePanel};
		
		x += m_TileSizePanel + 2;
		if (i % 32 == 0)
		{
			row++;
			x = 10;
			y = pan.y + 10 + (m_TileSizePanel + 2) * row;
		}

		// Check if the mouse is currently hovering over one and put blue box around that
		if (m_Cursor[0] > box.x && m_Cursor[0] < box.x + box.w && m_Cursor[1] > box.y && m_Cursor[1] < box.y + box.h)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			box.x -= 2;
			box.y -= 2;
			box.w = box.h = m_TileSizePanel + 4;
			SDL_RenderDrawRect(renderer, &box);

			// Set hover flag to true, and set id
			hover = true;
			m_HoverActive = i;
		}

		// If it is currently selected, put red box around it
		if (i == m_CurChar)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

			if (!hover || m_HoverActive != i)
			{
				box.x -= 2;
				box.y -= 2;
			}
			box.w = box.h = m_TileSizePanel + 4;
			SDL_RenderDrawRect(renderer, &box);
			
		}

		SDL_RenderFillRect(renderer, &box);

		// Draw buttons
		m_DrawButton.render(renderer);
		m_LineButton.render(renderer);
		m_RectButton.render(renderer);

		// Draw selection arrow for current tool
		switch (m_ActiveTool)
		{
		case Tool::DRAW:
			m_Arrow.render(m_DrawButton.getX() + m_DrawButton.getW() + 10, m_DrawButton.getY() + m_DrawButton.getH() / 2 - m_Arrow.getHeight() / 2);
			break;
		case Tool::LINE:
			m_Arrow.render(m_LineButton.getX() + m_LineButton.getW() + 10, m_LineButton.getY() + m_LineButton.getH() / 2 - m_Arrow.getHeight() / 2);
			break;
		case Tool::RECT:
			m_Arrow.render(m_RectButton.getX() + m_RectButton.getW() + 10, m_RectButton.getY() + m_RectButton.getH() / 2 - m_Arrow.getHeight() / 2);
			break;
		}

		// Draw Character in box scaled
		SDL_Rect clip = IdToClip(i);
		m_CharSheet.renderSize(box.x, box.y, m_TileSizePanel, m_TileSizePanel, &clip);
		
		if (!hover)
			m_HoverActive = -1;

		// Set color back to black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}
}

// CNTRL + SHIFT + Arrow Key decreases the grid on that side by one column or row
void CharGrid::decreaseGridSize(uint8_t dir)
{
	switch (dir)
	{
		// Up
	case 0:
		// Remove a row from the top of grid
		if (m_Height > 1)
		{
			m_Height -= 1;
			for (auto& c : m_Tiles)
			{
				c.erase(c.begin());
			}
		}
		break;
		// Down
	case 1:
		// Remove a row from the bottom of grid
		if (m_Height > 1)
		{
			m_Height -= 1;
			for (auto& c : m_Tiles)
			{
				c.erase(c.end() - 1);
			}
		}
		break;
		// Left
	case 2:
		// Remove a column from the left of grid
		if (m_Width > 1)
		{
			m_Width -= 1;
			m_Tiles.erase(m_Tiles.begin());
		}
		break;
		// Right
	case 3:
		// Remove a column from the the Right of grid
		if (m_Width > 1)
		{
			m_Width -= 1;
			m_Tiles.erase(m_Tiles.end() - 1);
		}
		break;
	}
}

void CharGrid::addLine()
{
	// Because lines can only be vertical or horizontal, whichever direction has the greatest distance will be the 
	// direction that counts for the line
	int dstX = m_Cursor[0] - m_DragStart[0];
	int dstY = m_Cursor[1] - m_DragStart[1];

	// Horizontal Line
	if (std::abs(dstX) > std::abs(dstY))
	{
		//tileAtMouse(m_Cursor[0], m_Cursor[1]) = m_CurChar;
		// If m_Cursor x was greater than m_DragStart x, start at m_DragStart
		if (dstX > 0)
		{
			for (int x = m_DragStart[0]; x < m_Cursor[0]; x += m_TileSize)
			{
				tileAtMouse(x, m_DragStart[1]) = m_CurChar;
			}
		}
		// Otherwise start at cursor
		else
		{
			for (int x = m_Cursor[0]; x < m_DragStart[0]; x += m_TileSize)
			{
				tileAtMouse(x, m_DragStart[1]) = m_CurChar;
			}
		}
	}
	// Vertical line
	else
	{
		//tileAtMouse(m_Cursor[0], m_Cursor[1]) = m_CurChar;
		// If m_Cursor y was greater than m_DragStart y, start at m_DragStart
		if (dstY > 0)
		{
			for (int y = m_DragStart[1]; y < m_Cursor[1]; y += m_TileSize)
			{
				tileAtMouse(m_DragStart[0], y) = m_CurChar;
			}
		}
		// Otherwise start at cursor
		else
		{
			for (int y = m_Cursor[1]; y < m_DragStart[1]; y += m_TileSize)
			{
				tileAtMouse(m_DragStart[0], y) = m_CurChar;
			}
		}
	}
}

void CharGrid::addRect()
{
	int dstX = m_Cursor[0] - m_DragStart[0];
	int dstY = m_Cursor[1] - m_DragStart[1];
	
	int startX = 0;
	int startY = 0;
	int endX = 0;
	int endY = 0;

	// Horizontal starting and ending point
	// If m_Cursor x was greater than m_DragStart x, start at m_DragStart
	if (dstX > 0)
	{
		startX = m_DragStart[0];
		endX = m_Cursor[0];
	}
	// Otherwise start at cursor
	else
	{
		startX = m_Cursor[0];
		endX = m_DragStart[0];
	}

	// Vertical starting and ending point
	// If m_Cursor y was greater than m_DragStart y, start at m_DragStart
	if (dstY > 0)
	{
		startY = m_DragStart[1];
		endY = m_Cursor[1];
	}
	// Otherwise start at cursor
	else
	{
		startY = m_Cursor[1];
		endY = m_DragStart[1];
	}

	for (int x = startX; x < endX + 2; x += m_TileSize + 2)
	{
		for (int y = startY; y < endY + 2; y += m_TileSize + 2)
		{
			tileAtMouse(x, y) = m_CurChar;
		}
	}
}

// CNTRL + SHIFT + Arrow Key increases the grid on that side by one column or row
void CharGrid::increaseGridSize(uint8_t dir)
{
	std::vector<uint8_t> col;
	switch (dir)
	{
		// Up
		case 0:
			// Add a new row to the top of grid
			m_Height += 1;
			for (auto& c : m_Tiles)
			{
				c.insert(c.begin(), 32);
			}
			break;
		// Down
		case 1:
			// Add a new row to the bottom of grid
			m_Height += 1;
			for (auto& c : m_Tiles)
			{
				c.insert(c.end(), 32);
			}
			break;
		// Left
		case 2:
			// Add a new column to the left of grid
			if (m_Width < 148) // Thanks @SendingThruHardware for limit info
			{
				m_Width += 1;
				for (int i = 0; i < m_Height; i++)
				{
					col.push_back(32);
				}
				m_Tiles.insert(m_Tiles.begin(), col);
			}
			break;
		// Right
		case 3:
			// Add a new column to the Right of grid
			if (m_Width < 148) // Thanks @SendingThruHardware for limit info
			{
				m_Width += 1;
				for (int i = 0; i < m_Height; i++)
				{
					col.push_back(32);
				}
				m_Tiles.insert(m_Tiles.end(), col);
			}
			break;	
	}
}



