#pragma once

enum TColor { WHITE, BLACK, BLUE, PINK, YELLOW };

struct Texture 
{
	GLuint				id;
	std::string			type;
	std::string			path;
	
	//Texture dimensions
	int32_t				mTextureWidth;
	int32_t				mTextureHeight;
	int32_t				mChannels			= 1;
	int32_t				numberofRows		= 1;
	
	Texture() { type = "default", path = "empty", id = (GLuint)glm::pow(2,8)- 1, mTextureWidth = 1, mTextureHeight = 1; loadTexture1x1(YELLOW); }
	Texture(GLuint Width, GLuint Height) :Texture() { mTextureWidth = Width;  mTextureHeight = Height; }
	Texture(GLuint ID, GLuint TextureWidth, GLuint TextureHeight) :id(ID), mTextureWidth(TextureWidth), mTextureHeight(TextureHeight) {  }
	Texture(const Texture& t)
	{
		this->operator=(t);
	}
	Texture& operator=(const Texture& t) 
	{ 
		id				= t.id, 
		mTextureWidth	= t.mTextureWidth, 
		mTextureHeight	= t.mTextureHeight, 
		type			= t.type,
		path			= t.path;
		mChannels		= t.mChannels;
		numberofRows	= t.numberofRows;
		return *this; 
	}

	virtual  bool loadTextureFromFile(const std::string& path, GLenum format =GL_RGBA, GLenum wrap = GL_CLAMP_TO_EDGE);
	virtual  bool saveToFile(const std::string &path);
	virtual  void freeTexture();
	virtual  bool loadTexture1x1(TColor color);
	virtual  bool loadCubemap(std::vector<std::string> faces);
	virtual  bool generatePerlin(GLuint Width, GLuint Height, bool periodic);
	virtual  void setNumRows(GLuint nrows) { numberofRows = nrows; }

	template<class GLtype>
	bool TextureFromBuffer(GLtype* buffer, GLuint Width, GLuint Height)
	{
		GLenum type ;
		if (std::is_same_v<GLtype, GLfloat> == true)
			type = GL_FLOAT;
		else if (std::is_same_v<GLtype, GLubyte> == true)
			type = GL_UNSIGNED_BYTE;
		//else
			//static assert

		this->mTextureWidth = Width;
		this->mTextureHeight = Height;
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &this->id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, this->mTextureWidth, this->mTextureHeight);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, type, buffer);
		mChannels = 4; //RGBA
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}
};



