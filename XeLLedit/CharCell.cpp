#include "CharCell.h"

void CharCell::setTile(uint8_t id)
{
	m_tileID = id;
}

uint8_t CharCell::getTile()
{
	return m_tileID;
}
