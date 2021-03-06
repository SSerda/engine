#include "stdafx.h"
#include "Camera.h"

Camera::Camera() : viewDirection(0.0f,0.0f,1.0f),Up(0.0f,1.0f,0.0f),position(2.0f,4.0f,-3.0f),strafeDirection(1.0f,0.0f,0.0f)
{}

Camera::Camera(const Camera & other)
{
	position		 = other.position;
	viewDirection	 = other.viewDirection;
	oldMousePosition = other.oldMousePosition;
	strafeDirection	 = other.strafeDirection;
	rotationMatrix	 = other.rotationMatrix;
}

Camera& Camera::operator=(const Camera& other)
{
	if (&other != this) 
	{  
		position		 = other.position;
		viewDirection	 = other.viewDirection;
		oldMousePosition = other.oldMousePosition;
		strafeDirection	 = other.strafeDirection;
		rotationMatrix	 = other.rotationMatrix;
	}
	return *this;
}

bool Camera::OnMouseMove(uint32_t x, uint32_t y)
{
	mouseUpdate(glm::vec2(x, y));
	return true;
}

bool Camera::OnKeyPress(uint32_t asci)
{
	switch(asci)
	{
		case ASCII_W: moveForward();  return true;
		case ASCII_S: moveBackword(); return true;
		case ASCII_D: strafeLeft();   return true;
		case ASCII_A: strafeRight();  return true;
		case ASCII_R: moveUp();		  return true;
		case ASCII_F: moveDown();	  return true;
		default:					  return false;
	}
	return false;
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, Up);
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

glm::vec3 Camera::getDirection() const
{
	return viewDirection;
}

 glm::mat3 Camera::getRotationMatrix() const
{
	 return this->rotationMatrix;
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if(glm::length(mouseDelta) > 5.0f)
	{
		oldMousePosition = newMousePosition;
	}
	else
	{
		strafeDirection = glm::cross(Up,viewDirection);
		viewDirection = glm::mat3(
			glm::rotate(-mouseDelta.x * MOVEMENT_SPEED * 0.1f , Up) * 
			glm::rotate(mouseDelta.y * MOVEMENT_SPEED * 0.1f, strafeDirection)) * viewDirection;
		oldMousePosition = newMousePosition;
		//
		rotationMatrix = rotationMatrix* glm::mat3(
			glm::rotate(-mouseDelta.x * MOVEMENT_SPEED * 0.1f, Up) *
			glm::rotate(mouseDelta.y * MOVEMENT_SPEED * 0.1f, strafeDirection));
	}
}

void Camera::moveForward() 
{
	position += MOVEMENT_SPEED * viewDirection;
}
void Camera::moveBackword()
{
	position -= MOVEMENT_SPEED * viewDirection;
}
void Camera::strafeLeft()
{
	position += -MOVEMENT_SPEED * strafeDirection;
}
void Camera::strafeRight()
{
	position += MOVEMENT_SPEED * strafeDirection;
}
void Camera::moveUp() 
{
	position += MOVEMENT_SPEED * Up;
}
void Camera::moveDown() 
{
	position += -MOVEMENT_SPEED * Up;
}