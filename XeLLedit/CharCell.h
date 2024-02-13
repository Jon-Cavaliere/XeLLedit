#pragma once
#include <stdint.h>
/*
	Class that represents a single character cell in a char grid
	Cells have:
		* CP473 Byte Value
		* UTF8 Byte Value
		* Tile ID
*/
class CharCell
{
public:
	void setTile(uint8_t id);
	uint8_t getTile();
private:
	uint8_t m_CP473;
	uint16_t m_UTF8;
	uint8_t m_tileID;
};

