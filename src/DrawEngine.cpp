#include "DrawEngine.h"
#include <iostream>

DrawEngine::DrawEngine() {}

// Initialize SDL_Image and SDL_TTF
bool DrawEngine::Init(GraphicEngine *gfx)
{
	bool success = true;
	renderer = gfx->GetRenderer();

	// init SDL_image
	int imgFlags = IMG_INIT_PNG; //Loading PNGs
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cerr << "SDL_image init FAILED : " << IMG_GetError() << std::endl;
		success = false;
	}

	// init SDL_ttf
	if (TTF_Init() == -1)
	{
		std::cerr << "SDL_ttf init FAILED : " << TTF_GetError() << std::endl;
		success = false;
	}
	font = TTF_OpenFont("media/fonts/Roboto-Medium.ttf", 28);
	if (font == NULL)
	{
		std::cerr << "Failed to open font : " << TTF_GetError() << std::endl;
		success = false;
	}

	return success;
}

void DrawEngine::Cleanup()
{
	// clean ressources map
	for (auto &pair : ressources)
	{
		SDL_Texture *texture = pair.second;
		if (texture != NULL)
		{
			SDL_DestroyTexture(texture);
		}
		texture = NULL;
	}
	ressources.clear();

	TTF_CloseFont(font);
	font = NULL;

	TTF_Quit();
	IMG_Quit();
}

// Imgfile -> Texture
SDL_Texture *DrawEngine::LoadImage(std::string imgPath)
{
	// Look for texture in ressources map.
	int const pos = imgPath.find_last_of('/');
	const std::string leaf = imgPath.substr(pos + 1);
	if (GetRessource(leaf) != NULL)
	{
		// Returns it directly when found
		return ressources.at(leaf);
	}

	// Load with SDL_Image
	SDL_Texture *newTexture = NULL;
	SDL_Surface *tempSurface = IMG_Load(imgPath.c_str());
	if (tempSurface == NULL)
	{
		std::cerr << "Failed to load " << imgPath << " : " << IMG_GetError() << std::endl;
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		if (newTexture == NULL)
		{
			std::cerr << "Failed to create texture from " << imgPath << " : " << SDL_GetError() << std::endl;
		}
		SDL_FreeSurface(tempSurface);
	}

	// Push into ressources map.
	ressources[leaf] = newTexture;
	return newTexture;
}

// Create a texture with your text.
SDL_Texture *DrawEngine::LoadText(std::string text, SDL_Color color)
{
	SDL_Texture *newTexture = NULL;
	SDL_Surface *tempSurface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
	if (tempSurface == NULL)
	{
		std::cerr << "Failed to render text surface : " << TTF_GetError() << std::endl;
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		if (newTexture == NULL)
		{
			std::cerr << "Failed to create texture from rendered text : " << SDL_GetError() << std::endl;
		}
		//Get rid of old surface
		SDL_FreeSurface(tempSurface);
	}

	return newTexture;
}

// Look into ressources map
SDL_Texture *DrawEngine::GetRessource(std::string key)
{
	int c = ressources.count(key);
	if (c == 0)
		return NULL;
	return ressources.at(key);
}

// 	void Load(std::string className, std::string file);