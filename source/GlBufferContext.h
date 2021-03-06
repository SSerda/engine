#pragma once

#include "colorFBO.h"
#include "ShadowMapFbo.h"
#include "GFBO.h"

enum class eBuffer
{
	BUFFER_DEFAULT,
	BUFFER_SHADOW,
	BUFFER_BRIGHT_FILTER,
	BUFFER_GAUSSIAN_ONE,
	BUFFER_GAUSSIAN_TWO,
	BUFFER_REFLECTION,
	BUFFER_REFRACTION,
	BUFFER_SCREEN,
	BUFFER_MTS,
	BUFFER_DEFFERED,
	BUFFER_DEFFERED1,
	BUFFER_DEFFERED2,
};

class eGlBufferContext
{
public:
	eGlBufferContext()								{}
	eGlBufferContext(const eGlBufferContext&)		= delete;
	eGlBufferContext& operator=(eGlBufferContext&)	= delete;
	void BufferInit(eBuffer, unsigned int, unsigned int);
	void EnableWrittingBuffer(eBuffer);
	void EnableReadingBuffer(eBuffer, GLenum slot);
	Texture GetTexture(eBuffer);
	void ResolveMtsToScreen() { mtsFBO.ResolveToFBO(&screenFBO); }
	void BlitDepthFromTo(eBuffer, eBuffer);
	static eGlBufferContext& GetInstance() 
	{
		static eGlBufferContext  instance;
		return instance;
	}

private:
	ShadowMapFBO	depthFBO;
	eColorFBO		screenFBO;
	eColorFBO		mtsFBO;
	eColorFBO		reflectionFBO;
	eColorFBO		refractionFBO;
	eColorFBO		brightFilterFBO;
	eColorFBO		gausian1FBO;
	eColorFBO		gausian2FBO;
	eGFBO			gFBO;
};
