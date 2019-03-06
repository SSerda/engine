#pragma once

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/norm.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm/gtc/constants.hpp>

class Transform
{
protected:
	glm::vec3 m_translation;
	glm::vec3 m_scale;
	glm::quat q_rotation;
	glm::vec3 forward		 = glm::vec3(0.f, 0.0f, 1.0f); 
	glm::vec3 Up			 = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 totalTransform = glm::mat4();

	void				UpdateModelMatrix();

public:
	Transform() { m_scale = glm::vec3(1.0f, 1.0f, 1.0f);  UpdateModelMatrix();}
	
	glm::mat4			getModelMatrix() const;

	void				setRotation(float x, float y, float z);
	void				setRotation(glm::quat q)	 { q_rotation = q;		UpdateModelMatrix(); }
	virtual void		setTranslation(glm::vec3 tr) { m_translation = tr;	UpdateModelMatrix(); }
	virtual void		setScale(glm::vec3 sc)		 { m_scale = sc;		UpdateModelMatrix(); }
	
	static glm::quat	RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
	static Transform	interpolate(const Transform& first, const Transform& second, float progression);
	
	virtual glm::mat4	getScale()			const { return  glm::scale(glm::vec3(m_scale.x, m_scale.y, m_scale.z)) ; }
	virtual glm::vec3	getTranslation()	const { return  m_translation; }
	virtual glm::quat	getRotation()		const { return  q_rotation; }
	glm::vec4			getRotationVector() const;
	glm::vec3			getForward()		const { return forward; }
	glm::vec3			getUp()				const { return Up; }
	
	void				incrementScale() { m_scale.x += 0.01f, m_scale.y += 0.01f, m_scale.z += 0.01f;  UpdateModelMatrix();}
	void				decrementScale() { m_scale.x -= 0.01f, m_scale.y -= 0.01f, m_scale.z -= 0.01f;  UpdateModelMatrix();}
	void				billboard(glm::vec3 direction);
	bool				turnTo(glm::vec3 dest, float speed);	
};