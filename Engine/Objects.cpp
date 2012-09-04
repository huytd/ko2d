#include "Objects.h"
#include "ResourcesManager.h"
#include "Camera.h"

Objects::Objects(void)
{
	Position = Vector3(0, 0, 0);
	Rotation = Vector3(0, 0, 0);
	Scale = Vector3(1, 1, 1);
	type = 0;
	coll = false;
}

Objects::~Objects(void)
{
}
void Objects::init(void)
{
	LOGI(" \n Created Object\n");
}

void Objects::init(int _model, int _texture, int _shader)
{
	frM = ResourcesManager::GetInstance()->getFrameModel(_model);
	modelID = ResourcesManager::GetInstance()->getModel(_model)->vboID;
	modelVerNum = ResourcesManager::GetInstance()->getModel(_model)->numCount;
	textureID = ResourcesManager::GetInstance()->getTexture(_texture)->texID;
	shaderID = ResourcesManager::GetInstance()->getShader(_shader)->program;
}

void Objects::Draw()
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	if (modelID != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, modelID);

		if (shaderID != -1)
		{
			glUseProgram(shaderID);

			GLuint loc = glGetUniformLocation(shaderID, "u_mvpMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat*)mvpMatrix.m);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID);
			GLuint uTexLoc = glGetUniformLocation(shaderID, "u_texture");
			glUniform1i(uTexLoc, 0);

			GLuint apos = glGetAttribLocation(shaderID, "a_posL");
			if(apos != -1)
			{
				glEnableVertexAttribArray(apos);
				glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			}

			GLuint texCorLoc = glGetAttribLocation(shaderID, "a_uv");
			if(texCorLoc != -1)
			{
				glEnableVertexAttribArray(texCorLoc);
				glVertexAttribPointer(texCorLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
			}

		}

		glDrawArrays(GL_TRIANGLES, 0, modelVerNum);

		glBindBuffer(GL_ARRAY_BUFFER, 0);	
	}
}

void Objects:: UpdateFrame()
{
	frame.max_x = Position.x + (*frM).max_x * Scale.x;
	frame.min_x = Position.x + (*frM).min_x * Scale.x;
	frame.max_y = Position.y + (*frM).max_y * Scale.y;
	frame.min_y = Position.y + (*frM).min_y * Scale.y;
	frame.max_z = Position.z + (*frM).max_z * Scale.z;
	frame.min_z = Position.z + (*frM).min_z * Scale.z;
}
void Objects::Update(float deltaTime)
{
	getDeltaTime = deltaTime;

	Matrix mpos, mrotx, mroty, mrotz, mrot, mview, mproj, mscale;
	mscale.SetScale(Scale);
	mpos.SetTranslation(Position);
	mrotx.SetRotationX(Rotation.x);
	mroty.SetRotationY(Rotation.y);
	mrotz.SetRotationZ(Rotation.z);
	mrot = mroty * mrotx * mrotz;
	mvpMatrix = mscale * mrot * mpos * Camera::GetInstance()->getViewMatrix() * Camera::GetInstance()->getProjectionMatrix();
}

