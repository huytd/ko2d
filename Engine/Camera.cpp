#include "Camera.h"
#include <time.h>

Camera::Camera()
{
	isShake = false;
	shakeTimer = 0;

	shake = 0.0f;

	moveSpeed = 1.0f;
	rotSpeed = 0.3f;
	moveDirection = Vector3(0, 0, 0);
	viewMatrix.SetIdentity();
	rotationMatrix.SetIdentity();
	positionMatrix.SetIdentity();
	projectionMatrix.SetPerspective(45.0f, (float)SCREEN_W / (float)SCREEN_H, 0.1f, 1000.0f);
	OldPosition = Position;
}
void Camera::Draw()
{
}

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void Camera::Update(float deltaTime)
{
	if (isShake)
	{
		shake += 80*deltaTime;
		Position.y += cos(shake)/50;
		Position.x += sin(shake)/50;
		Position.z += cos(shake)/50;

		shakeTimer -= 1;
		if (shakeTimer <= 0) {
			isShake = false;
			Position = OldPosition;
		}
	}
	else
	{
		OldPosition = Position;
	}

	mcrX.SetRotationX( Rotation.x);
	mcrY.SetRotationY( Rotation.y);
	mcrZ.SetRotationZ( Rotation.z);
	Vector4 deltaT = (mcrZ * mcrX * mcrY) * Vector4(moveDirection);
	Position.x += deltaT.x;
	Position.y += deltaT.y;
	Position.z += deltaT.z;

	// process camera
	mcrX.SetRotationX(-Rotation.x);
	mcrY.SetRotationY(-Rotation.y);
	mcrZ.SetRotationZ(-Rotation.z);
	rotationMatrix = mcrY * mcrX * mcrZ;
	positionMatrix.SetTranslation(-Position);
	viewMatrix = positionMatrix * rotationMatrix;
}

void Camera::shakeCamera(int t)
{
	isShake = true;
	shakeTimer = t;
}

void Camera::Key(float deltaTime, unsigned int curKey)
{
	if (curKey == 37) { // key left: rotate camY
		this->Yaw(deltaTime);
	}
	if (curKey == 39) { // key right: rotate camY
		this->Yaw(-deltaTime);
	}
	if (curKey == 38) { // key up: rotate camX
		this->Pitch(deltaTime);
	}
	if (curKey == 40) { // key down: rotate camX
		this->Pitch(-deltaTime);
	}
	if (curKey == 65) { // key A: move camera left
		this->MoveLeft(deltaTime);
	}
	if (curKey == 68) { // key D: move camera right
		this->MoveRight(deltaTime);
	}
	if (curKey == 87) { // key W: zoom in
		this->MoveForward(deltaTime);
	}
	if (curKey == 83) { // key S: zoom out
		this->MoveBackward(deltaTime);
	}
	if (curKey == 0) {
		this->Stop();
	}
}

Matrix Camera::getViewMatrix()
{
	return viewMatrix;
}

void Camera::Stop()
{
	moveDirection = Vector3(0, 0, 0);
}

void Camera::MoveForward(float step)
{
	moveDirection = Vector3(0, 0, -moveSpeed * step);
}

void Camera::MoveBackward(float step)
{
	moveDirection = Vector3(0, 0, moveSpeed * step);
}

void Camera::MoveLeft(float step)
{
	moveDirection = Vector3(-moveSpeed * step, 0, 0);
}

void Camera::MoveRight(float step)
{
	moveDirection = Vector3(moveSpeed * step, 0, 0);
}

void Camera::Pitch(float angle) // Rotate X
{
	setRotation(getRotation().x + angle * rotSpeed, getRotation().y, getRotation().z);
}

void Camera::Roll(float angle) // Rotate Z
{
	setRotation(getRotation().x, getRotation().y, getRotation().z + angle * rotSpeed);
}

void Camera::Yaw(float angle) // Rotate Y
{
	setRotation(getRotation().x, getRotation().y + angle * rotSpeed, getRotation().z);
}

void Camera::setMoveSpeed(float mov)
{
	moveSpeed = mov;
}
float Camera::getMoveSpeed()
{
	return moveSpeed;
}

void Camera::setRotateSpeed(float rot)
{
	rotSpeed = rot;
}
float Camera::getRotateSpeed()
{
	return rotSpeed;
}