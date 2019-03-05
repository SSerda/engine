#ifndef MAIN_CONTEXT_H
#define MAIN_CONTEXT_H

#include <GL\glew.h>
#include "Structures.h"
#include "TerrainModel.h"
#include "Camera.h"
//#include "Timer.h"
#include "CameraRay.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "RenderManager.h"
#include "GUI.h"

class SoundContext;
class remSnd;

class eMainContext 
{
public:
	eMainContext();
	virtual ~eMainContext();

	void			InitializeGL();
	void			PaintGL();
	void			UpdateLight(uint x, uint y, uint z);

	Camera&					GetCamera()			{ return m_camera;		}
	dbb::CameraRay&			GetCameraRey()		{ return camRay;		}
	shObject&				GetFocusedObject()	{ return m_focused;		}
	std::vector<shObject>&  GetObjects()		{ return m_Objects;		}
	std::vector<GUI>&		GetGuis()			{ return guis;			}
	uint					Width()				{ return width;			}
	uint					Height()			{ return height;		}
	float					WaterHeight()		{ return waterHeight;	}

private:
	//std::unique_ptr<dbb::Timer>			dTimer;
	Camera								m_camera;
	dbb::CameraRay						camRay;
	
	shObject							m_focused;
	std::vector<shObject>				m_Objects;
	std::vector<shObject>				m_framed;
	Light								m_light;
	shObject							lightObject; //debuging
	std::unique_ptr<TerrainModel>		m_TerrainModel;
	
	std::unique_ptr<SoundContext>		context; //test
	std::unique_ptr<remSnd>				sound;  //test
	std::vector<GUI>					guis;
	//managers
	TextureManager						texManager;
	ModelManager						modelManager;
	eRenderManager						renderManager;

	float								waterHeight = 2.0f;
	bool								mts			= true;
	uint								width		= 1200;
	uint								height		= 600;
	Texture								text;
	bool								mousepress = false;
	GLenum								drawBufs[1];

protected:
	void								InitializeBuffers();
	void								InitializeModels();
	void								InitializeRenders();
	void								Pipeline();
};

#endif