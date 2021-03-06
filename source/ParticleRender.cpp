#include "stdafx.h"
#include "ParticleRender.h"
#include "ParticleSystem.h"
#include "Object.h"
#include "MyModel.h"
#include "MyMesh.h"
#include "Transform.h"
#include "ShapeGenerator.h"
#include "ShapeData.h"

eParticleRender::eParticleRender(std::shared_ptr<MyMesh> _mesh,
								 Texture*				 _texture,
								 const string&			 _vertexShaderPath,
								 const string&			 _fragmentShaderPath)
: instancedBuffer(ParticleMesh::MAXPARTICLES  * ParticleMesh::SIZEOF / sizeof(float), 0.0f)
{
	particleShader.installShaders(_vertexShaderPath.c_str(), _fragmentShaderPath.c_str());

	modelToWorldMatrixUniformLocation		= glGetUniformLocation(particleShader.ID, "modelToWorldMatrix");
	fullTransformationUniformLocation		= glGetUniformLocation(particleShader.ID, "modelToProjectionMatrix");
	worldToProjectionMatrixUniformLocation	= glGetUniformLocation(particleShader.ID, "worldToProjectionMatrix");
	ProjectionMatrixUniformLocation			= glGetUniformLocation(particleShader.ID, "ProjectionMatrix");
	modelViewMatrixLocation					= glGetUniformLocation(particleShader.ID, "ModelViewMatrix");
	texOffset1Location						= glGetUniformLocation(particleShader.ID, "texOffset1");
	texOffset2Location						= glGetUniformLocation(particleShader.ID, "texOffset2");
	texCoordInfoLocation					= glGetUniformLocation(particleShader.ID, "texCoordInfo");

	model.reset(new MyModel(_mesh, _texture));
	object.reset(new eObject(model.get())); // prob transform will do

	ShapeData quad	= ShapeGenerator::makeQuad();
	mesh.reset(new ParticleMesh(quad));
	quad.cleanup();
}

void eParticleRender::Render(const glm::mat4 & _projectionMatrix, const Camera & _camera)
{
	glUseProgram(particleShader.ID);
	glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, &_projectionMatrix[0][0]);

	int info = 0;
	for(auto& system : systems)
	{
		object->getTransform()->billboard(_camera.getDirection());
		object->getTransform()->setScale(system->Scale());

		system->generateParticles();  

		int counter		= 0;
		int instances	= 0;
		std::vector<Particle>::iterator n_end = system->prepareParticles(_camera.getPosition());
		std::vector<Particle>::iterator iter = system->getParticles().begin();
		if(iter != system->getParticles().end() && n_end != system->getParticles().begin()) 
		{
			while(iter != n_end) 
			{
				instances++;
				object->getTransform()->setTranslation(iter->getPosition());
				mat4 modelViewMatrix = _camera.getWorldToViewMatrix() * object->getTransform()->getModelMatrix();
				glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);  //modelView
				
				LoadOffsetsInfo(iter->gettexOffset1(), iter->gettexOffset2(), iter->getNumRows(), iter->getBlend());

				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < 4; ++j) {
						instancedBuffer[counter++] = modelViewMatrix[i][j]; // [i][j] ? [j][i]
					}
				}
				instancedBuffer[counter++] = iter->gettexOffset1().x;
				instancedBuffer[counter++] = iter->gettexOffset1().y;
				instancedBuffer[counter++] = iter->gettexOffset2().x;
				instancedBuffer[counter++] = iter->gettexOffset2().y;
				instancedBuffer[counter++] = iter->getNumRows();
				instancedBuffer[counter++] = iter->getBlend();

				++iter;
			}
		}	
		mesh->updateInstancedData(instancedBuffer);
		mesh->SetUpInstances(instances);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, system->GetTexture()->id);
		glUniform1i(glGetUniformLocation(particleShader.ID, "texture_diffuse1"), 2);
		mesh->Draw();

		++info;
	}
	auto toRemove = std::remove_if(systems.begin(), systems.end(),[](std::shared_ptr<IParticleSystem> sys){return sys->IsFinished();});
	if(toRemove != systems.end())
	{
		systems.erase(toRemove);
	}
}
   
void eParticleRender::AddParticleSystem(IParticleSystem* sys)
{
	systems.push_back(std::shared_ptr<IParticleSystem>(sys));
}

void eParticleRender::LoadOffsetsInfo(glm::vec2 offset1, glm::vec2 offset2, float numRows, float blend)
{
	glUniform2f(texOffset1Location, offset1[0], offset1[1]);
	glUniform2f(texOffset2Location, offset2[0], offset2[1]);
	glUniform2f(texCoordInfoLocation, numRows, blend);
}
