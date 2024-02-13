#include "Texture.h"
#include <stdexcept>

Texture::Texture() :
	m_Texture(nullptr), p_Renderer(nullptr), m_Width(0), m_Height(0)
{
}

Texture::~Texture()
{
	// Deallocate
	free();
}

bool Texture::loadFromFile(std::string path, bool colorkey = true, SDL_Renderer* gRenderer = nullptr)
{
	// Get rid of preexisting texture
	free();

	// Set renderer reference if not nullptr
	if (gRenderer)
		p_Renderer = gRenderer;

	// If they are both invalid pointer, throw exception
	if (!p_Renderer)
	{
		char const* errMsg = "Renderer is nullptr (in Texture.cpp::loadFromFile())";

		throw std::runtime_error(std::string("Failed: ") + errMsg);
	}

	// The final texture
	SDL_Texture* newTexture = NULL;

	// Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		if(colorkey)
			// Remove black background
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));

		// Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(p_Renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			// Get image dimensions
			m_Width = loadedSurface->w;
			m_Height = loadedSurface->h;
			printf("Successfully loaded image: %s\n", path.c_str());
		}

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	// Return success
	m_Texture = newTexture;
	return m_Texture != NULL;
}

void Texture::free()
{
	// Free texture if it exists
	if (m_Texture)
	{
		SDL_DestroyTexture(m_Texture);
		m_Texture = NULL;
		m_Width = 0;
		m_Height = 0;
	}
}

void Texture::render(int32_t x, int32_t y, SDL_Rect* clip)
{
	// Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, m_Width, m_Height };

	// Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	// Render to screen if renderer isn't null
	if (p_Renderer)
		SDL_RenderCopy(p_Renderer, m_Texture, clip, &renderQuad);
	// If renderer is nullptr, throw exception
	else
	{
		char const* errMsg = "Renderer is nullptr (in Texture.cpp::render())";
		throw std::runtime_error(std::string("Failed: ") + errMsg);
	}

}


void Texture::renderSize(int32_t x, int32_t y, int32_t w, int32_t h, SDL_Rect* clip)
{
	// Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, m_Width, m_Height };

	// Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = w;
		renderQuad.h = h;
	}

	// Render to screen if renderer isn't null
	if (p_Renderer)
		SDL_RenderCopy(p_Renderer, m_Texture, clip, &renderQuad);
	// If renderer is nullptr, throw exception
	else
	{
		char const* errMsg = "Renderer is nullptr (in Texture.cpp::render())";

		throw std::runtime_error(std::string("Failed: ") + errMsg);
	}
}

int Texture::getWidth()
{
	return m_Width;
}

int Texture::getHeight()
{
	return m_Height;
}

void Texture::setRenderer(SDL_Renderer* renderer)
{
	p_Renderer = renderer;
}
