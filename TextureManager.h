#pragma once
#include <map>
#include "Texture.h"

class TextureManager
{
private:
	std::map<std::string, Texture> m_Textures;
public:
	void loadAllTextures();
	Texture* find(std::string);
};