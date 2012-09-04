#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Engine.h"
#include "Vertex.h"
#include "Singleton.h"

class Camera: public Singleton<Camera>
{
private:
	float moveSpeed;
	float rotSpeed;
	Vector3 moveDirection;
	Matrix mcrX, mcrY, mcrZ, viewMatrix, rotationMatrix, positionMatrix, projectionMatrix;
	Vector3 Position, OldPosition;
	Vector3 Rotation;
	Vector3 Scale;
	float shake;
	int shakeTimer;
public:
	bool isShake;
	Camera();
	void Draw();
	void Update(float deltaTime);
	void Key(float deltaTime, unsigned int curKey);
	Matrix getViewMatrix();
	void Stop();
	void MoveForward(float step = 1.0f);
	void MoveBackward(float step = 1.0f);
	void MoveLeft(float step = 1.0f);
	void MoveRight(float step = 1.0f);
	void Roll(float angle = 1); // Rotate Z
	void Pitch(float angle = 1); // Rotate X
	void Yaw(float angle = 1); // Rotate Y
	void setRotateSpeed(float rot);
	void setMoveSpeed(float mov);
	float getMoveSpeed();
	float getRotateSpeed();
	void shakeCamera(int t);
	void setPosition(Vector3 pos)
	{
		Position = pos;
	}
	void setPosition(float x, float y, float z)
	{
		Position = Vector3(x, y, z);
	}
	Vector3 getPosition()
	{
		return Position;
	}
	void setRotation(Vector3 rot)
	{
		Rotation = rot;
	}
	void setRotation(float x, float y, float z)
	{
		Rotation = Vector3(x, y, z);
	}
	Vector3 getRotation() 
	{
		return Rotation;
	}
	void setScale(Vector3 sca)
	{
		Scale = sca;
	}
	void setScale(float x, float y, float z)
	{
		Scale = Vector3(x, y, z);
	}
	Vector3 getScale() 
	{
		return Scale;
	}
	void setProjectionMatrix (Matrix m)
	{
		projectionMatrix = m;
	}
	Matrix getProjectionMatrix()
	{
		return projectionMatrix;
	}
};

#endif