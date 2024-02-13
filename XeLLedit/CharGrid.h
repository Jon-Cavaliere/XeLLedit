#pragma once
#include <stdint.h>
#include <vector>
#include <SDL.h>
#include "Texture.h"
#include "Input.h"
#include "Vec2i.h"
#include "Button.h"

class CharGrid
{
public:
	CharGrid(uint16_t w, uint16_t h, uint16_t winWidth = 1280, uint16_t winHeight = 720);
	~CharGrid();
	void init(SDL_Renderer* renderer);
	void update(JC::Input* input);
	void render(SDL_Renderer* renderer);
	
	// Returns a reference to the tile array
	std::vector<std::vector<uint8_t>>& getTiles();

	void save(std::string filename);
	void load(std::string filename);

	void setWindow(SDL_Window* window);
private:
	uint16_t m_Width;
	uint16_t m_Height;

	uint16_t m_WinWidth;
	uint16_t m_WinHeight;

	bool m_Initialized;
	bool m_ShowGrid;

	// Tool constants
	enum Tool {DRAW = 0, LINE, RECT};
	Tool m_ActiveTool;

	// If tool is line or rect, flag to keep track of mouse being dragged, and start coords
	bool m_MouseDrag;
	JC::Vec2i m_DragStart;
	
	// Stores the value of whatever character is hovered in the selection panel (-1 = no hover)
	int16_t m_HoverActive;
	// Size of the previews for the characters in the selection panel 
	uint8_t m_TileSizePanel; 
	
	// Texture for the entire spritesheet (Cells will just get references or clip IDs)
	Texture m_CharSheet;

	// TileID for the current character
	uint8_t m_CurChar; 

	// Coordinates of the mouse
	JC::Vec2i m_Cursor;

	// Dimension of character tile in pixels
	uint8_t m_TileSize;

	// Array of tileIDs
	std::vector<std::vector<uint8_t>> m_Tiles;

	// Coordinates for where to begin rendering top left corner of the grid
	JC::Vec2i m_StartCoords;

	// Speed at which the grid is translated
	uint8_t m_MoveSpeed;

	// Selection panel buttons
	Button m_DrawButton;
	Button m_LineButton;
	Button m_RectButton;

	// Texture arrow that points at currently active tool
	Texture m_Arrow;

	// Internal method to convert the tileID to the clip coords on the character sprite sheet
	SDL_Rect IdToClip(uint8_t id);
	SDL_Rect IdToClip(); //  Same thing but lazy, just returns it for the currently selected tileid

	// Internal method for converting screen pixel to tile grid reference
	uint8_t& tileAtMouse(uint32_t x, uint32_t y);

	void tileCoordAtMouse(uint32_t mouseX, uint32_t mouseY, uint32_t& tileX, uint32_t& tileY);

	// Clear the entire grid (Set to spaces)
	void clear();

	// Set the entire grid to tile
	void setAll(uint8_t id);

	// Render just the selection panel
	void renderPanel(SDL_Renderer* renderer);

	// Pointer to the window
	SDL_Window* p_MainWindow;

	// Increase  grid size by one
	// up = 0, down = 1, left = 2, right = 3 
	void increaseGridSize(uint8_t dir);
	// decrease grid size by one
	// up = 0, down = 1, left = 2, right = 3 
	void decreaseGridSize(uint8_t dir);

	// Adds a line from m_DragStart to m_Cursor using currently selected character
	void addLine();

	// Adds a rect from m_DragStart to m_Cursor using currently selected character
	void addRect();
	
};

