
/**
	This program is for editing the ascii art displayed at the top of XeLL Reloaded.
	It generates a fully formatted, ready to go "asciiart.h" for inclusion when creating
	custom builds of XeLL Reloaded

	-- Codeus 

	Special Thanks To:
	* Cheez for idea and guidance
	* SendingThruHardware for testing and width limit
*/

#include "App.h"
#include "CP473_UTF16.h"
#include <cstdio>
#include <stdint.h>

int main(int argc, char** argv)
{
	App XellEditor;
	XellEditor.run();
	return 0;
}