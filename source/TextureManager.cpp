#include "stdafx.h"
#include "TextureManager.h"

void TextureManager::loadAllTextures()
{
	Texture text;
	text.loadTextureFromFile("bricks.jpg");
	m_Textures.insert(std::pair<std::string, Texture>("Tbricks0_d", text));
	text.loadTextureFromFile("container2.png");
	m_Textures.insert(std::pair<std::string, Texture>("Tcontainer0_d", text));
	text.loadTextureFromFile("grass.png");
	m_Textures.insert(std::pair<std::string, Texture>("Tgrass0_d", text));
	text.loadTextureFromFile("container2_specular.png");
	m_Textures.insert(std::pair<std::string, Texture>("Tcontainer0_s", text));
	text.loadTexture1x1(WHITE);
	m_Textures.insert(std::pair<std::string, Texture>("Twhite", text));
	text.loadTexture1x1(BLACK);
	m_Textures.insert(std::pair<std::string, Texture>("Tblack", text));
	text.loadTexture1x1(BLUE);
	m_Textures.insert(std::pair<std::string, Texture>("Tblue", text));
	text.loadTexture1x1(PINK);
	m_Textures.insert(std::pair<std::string, Texture>("Tpink", text));
	text.loadTextureFromFile("brickwall.jpg");//"brickwall.jpg"
	m_Textures.insert(std::pair<std::string, Texture>("Tbrickwall0_d", text));
	text.loadTextureFromFile("brickwall_normal.jpg");
	m_Textures.insert(std::pair<std::string, Texture>("Tbrickwall0_n", text));
	text.loadTextureFromFile("heightmap.png");
	m_Textures.insert(std::pair<std::string, Texture>("Theightmap0_n", text));
	text.loadTextureFromFile("waterDuDV.png");
	m_Textures.insert(std::pair<std::string, Texture>("TwaterDUDV", text));
	text.loadTextureFromFile("DUDVwaves.png");//, GL_RGBA, GL_REPEAT
	m_Textures.insert(std::pair<std::string, Texture>("Twaves0_n", text));
	//std::cout << "heightmap" << std::endl;
	text.loadTextureFromFile("heightmap.png");
	m_Textures.insert(std::pair<std::string, Texture>("Theightmap0_n", text));
	text.generatePerlin(255, 255, true); //HARDCODING!
	m_Textures.insert(std::pair<std::string, Texture>("Tperlin_n", text));
	text.loadTextureFromFile("spanish.jpg");
	m_Textures.insert(std::pair<std::string, Texture>("TSpanishFlag0_s", text));
	//std::cout << "atlas" << std::endl;
	text.loadTextureFromFile("atlas2.png");
	//std::cout << "atlas" << std::endl;
	m_Textures.insert(std::pair<std::string, Texture>("TfireAtlas0_n", text));
	text.loadTextureFromFile("shooting.jpg");
	m_Textures.insert(std::pair<std::string, Texture>("TShootAtlas0_n", text));
	text.loadTextureFromFile("penguins.jpg");
	m_Textures.insert(std::pair<std::string, Texture>("Tpenguins", text));
	text.loadTextureFromFile("bricks2.jpg");
	m_Textures.insert(std::pair<std::string, Texture>("Tbricks2_d", text));
	text.loadTextureFromFile("bricks2_normal.jpg");
	m_Textures.insert(std::pair<std::string, Texture>("Tbricks2_n", text));
	text.loadTextureFromFile("bricks2_depth.jpg");
	m_Textures.insert(std::pair<std::string, Texture>("Tbricks2_dp", text));
	text.loadTextureFromFile("pirate.png");
	m_Textures.insert(std::pair<std::string, Texture>("TPirate_flag0_s", text));
	text.loadTextureFromFile("ocean1.jpg");
	m_Textures.insert(std::pair<std::string, Texture>("TOcean0_s", text));
	text.loadTextureFromFile("bloom_test.jpg");
	m_Textures.insert(std::pair<std::string, Texture>("Tbloom_test", text));
	// Cubemap (Skybox)
	std::vector<std::string> faces;
	faces.push_back("right.jpg");
	faces.push_back("left.jpg");
	faces.push_back("top.jpg");
	faces.push_back("bottom.jpg");
	faces.push_back("back.jpg");
	faces.push_back("front.jpg");
	text.loadCubemap(faces);
	m_Textures.insert(std::pair<std::string, Texture>("TcubeSkyWater1", text));

}

Texture* TextureManager::find(std::string texture_name)
{
	if (m_Textures.find(texture_name) != m_Textures.end())
		return &m_Textures.find(texture_name)->second;
	else {
		std::cout << "texture not found!" << std::endl;
		return nullptr;
	}
}
