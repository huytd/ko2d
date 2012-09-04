#pragma once
#include "Singleton.h"
#include "3DMath.h"
#include "Vertex.h"
#include "Shaders.h"
#include "FileSystem.h"
#include "ResourcesManager.h"
#include "Camera.h"
#include <string>
#include <fstream>
using namespace std;

struct Rectangle
{
	float x; float y; float w; float h;
	float id;
};

struct TexCoord
{
	Vector2 tex[4];
	int id;
};

class GL2D: public Singleton<GL2D>
{
public:
	GLuint fontTexture, fontShaderID;
	TexCoord *texCoord;
	int countCoord;

	GL2D(void)
	{
	}

	~GL2D(void)
	{
		delete[] texCoord;
		texCoord = NULL;
	}

	// Draw sprite and animated sprite
	static void DrawBillboard(int textID, float x, float y, float z, int tx, int ty, int tw, int th)
	{
		GLuint shaderID = ResourcesManager::GetInstance()->getShader(5)->program;
		GLuint texID = ResourcesManager::GetInstance()->getTexture(textID)->texID;

		glEnable(GL_DEPTH_TEST);
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
		
		glUseProgram(shaderID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		GLuint uTexLoc = glGetUniformLocation(shaderID, "u_texture");
		glUniform1i(uTexLoc, 0);

		Matrix mvpMatrix, posMatrix, rotYMat, rotXMat;
		Vector3 position = Vector3( x, y, z );
		Vector3 direction = position - Camera::GetInstance()->getPosition();
		float angleY = tanf(direction.x / direction.z);
		float angleX = tanf(direction.y / -direction.z);
		posMatrix.SetTranslation( position );
		rotYMat.SetRotationY(angleY);
		rotXMat.SetRotationX(angleX);
		mvpMatrix = rotXMat * rotYMat * posMatrix * Camera::GetInstance()->getViewMatrix() * Camera::GetInstance()->getProjectionMatrix();
		GLuint loc = glGetUniformLocation(shaderID, "u_mvpMatrix");
		glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat*)mvpMatrix.m);

		Vector3 vdat[4];
		float buff[] = {
			-1, 1, 0, 
			-1, -1, 0,
			1, -1, 0,
			1, 1, 0
		};
		memcpy(&vdat, buff, sizeof(buff));
		GLuint posLoc = glGetAttribLocation(shaderID, "a_position");
		if(posLoc != -1)
		{
			glEnableVertexAttribArray(posLoc);
			glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), vdat);
		}

		Vector2 tdat[4];
		Vector2 ts = ScreenPosToTex(tw, th, 1024);
		float tbuff[] = {
			tx * ts.x, ty * ts.y,
			tx * ts.x, ty * ts.y + ts.y, 
			tx * ts.x + ts.x, ty * ts.y + ts.y,
			tx * ts.x + ts.x, ty * ts.y
		};
		memcpy(&tdat, tbuff, sizeof(tbuff));
		GLuint texCorLoc = glGetAttribLocation(shaderID, "a_uv");
		if(texCorLoc != -1)
		{
			glEnableVertexAttribArray(texCorLoc);
			glVertexAttribPointer(texCorLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), tdat);
		}
		int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, indices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Bitmap font
	bool LoadFont(const char* fileName, int shader = 0)
	{
		fontShaderID = ResourcesManager::GetInstance()->getShader(shader)->program;
		int imgSize = 0;
		int count = 0;
		int curr = 0;
	
		char* fileToRead = (char*)fileName;
		#ifdef _ANDROID
		LOGI("\nAndroid detected\n");
		fileToRead[strlen(fileToRead)-1] = '\0';
		#endif
		ifstream f(FileSystem::GetInstance()->GetPath(fileToRead).c_str());
		if (f)
		{
			while (!f.eof())
			{
				string s;
				getline(f, s);
				if (s.substr(0, 1) != " ") // not a blank line
				{
					if (s.substr(0, 2) == "fi") // file
					{
						// load texture file
						glGenTextures(1, &fontTexture);
						glBindTexture(GL_TEXTURE_2D, fontTexture);
						int imgWidth, imgHeight, imgBpp;
						char* imgData = LoadTGA(FileSystem::GetInstance()->GetPath("Fonts/font_0.tga").data(), &imgWidth, &imgHeight, &imgBpp);
						printf("Font texture loaded\nWidth: %d\nHeight: %d\nBpp: %d\n", imgWidth, imgHeight, imgBpp);
						glTexImage2D(GL_TEXTURE_2D, 0, (imgBpp==32)?GL_RGBA:GL_RGB, imgWidth, imgHeight, 0, (imgBpp==32)?GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE, imgData);
						glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
						glGenerateMipmap(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, 0);
						delete[] imgData;
					}
					if (s.substr(0, 2) == "si") // size
					{
						// get image size
						imgSize = atoi(s.substr(s.find(" ")+1).data());
						LOGI("\n IMG SIZE: %d", imgSize);
					}
					if (s.substr(0, 2) == "co") // count
					{
						// count
						count = atoi(s.substr(s.find(" ")+1).data());
						LOGI("\n COUNT: %d", count);
						texCoord = new TexCoord[count];
					}
					if (s.substr(0, 2) == "ch") // char
					{
						// char info
						float x, y, w, h;
						int id;
						sscanf(s.data(), "char id=%d \tx=%f \ty=%f \twidth=%f \nheight=%f", &id, &x, &y, &w, &h);
						x = x / imgSize; y = y / imgSize; w = w / imgSize; h = h / imgSize;
						texCoord[curr].tex[0] = Vector2(x, y);
						texCoord[curr].tex[1] = Vector2(x, y + h);
						texCoord[curr].tex[2] = Vector2(x + w, y + h);
						texCoord[curr].tex[3] = Vector2(x + w, y);
						texCoord[curr].id = id;
						curr++; // next line
					}
					LOGI("\n%s", s.data());
				}
			}
		}
		f.close();
		countCoord = curr;
		return true;
	}

	// Đếm số kí tự trong một chuỗi
	int count_char(const char* str)
	{
		int c = 0;
		while(str[c]) c++;
		return c;
	}

	// Dùng cho font, dò tìm texcoord của một kí tự
	int lookID(int id)
	{
		for (int i = 0; i < countCoord; i++)
		{
			if (texCoord[i].id == id) return i;
		}
		return -1;
	}

	// Vẽ một kí tự lên tọa độ x, y
	void DrawChar(int num, int x, int y, bool notlook = false, int csize = 50)
	{
		int id;
		if (!notlook)
			id = lookID(num);
		else id = num;
		if (id != -1)
		{
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glUseProgram(fontShaderID);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fontTexture);
			GLuint uTexLoc = glGetUniformLocation(fontShaderID, "u_texture");
			glUniform1i(uTexLoc, 0);

			float tsize = (float)csize / (float)SCREEN_W;
			float aspect = (float)SCREEN_W / (float)SCREEN_H;
			Vector2 p = ScreenPosToPos(x, y);
			Vector3 vdat[4];
			float buff[] = {
				p.x, p.y, 1.0,
				p.x, p.y - tsize, 1.0,
				p.x + tsize/aspect, p.y - tsize, 1.0,
				p.x + tsize/aspect, p.y, 1.0
			};
			memcpy(&vdat, buff, sizeof(buff));
			GLuint posLoc = glGetAttribLocation(fontShaderID, "a_position");
			if(posLoc != -1)
			{
				glEnableVertexAttribArray(posLoc);
				glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), vdat);
			}
			GLuint texCorLoc = glGetAttribLocation(fontShaderID, "a_uv");
			if(texCorLoc != -1)
			{
				glEnableVertexAttribArray(texCorLoc);
				glVertexAttribPointer(texCorLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), texCoord[id].tex);
			}
			int indices[] = {
				0, 1, 2,
				2, 3, 0
			};
			glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, indices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	// Vẽ một chuỗi lên tọa độ x, y
	void DrawString(const char* str, int x, int y, int size = 50)
	{
		float aspect = (float)SCREEN_W / (float)SCREEN_H;
		for (int i = 0; i < count_char(str); i++)
		{
			if (str[i] != ' ')
				DrawChar(str[i], (x + i*size/(aspect*aspect)), y, false, size);
		}
	}

	// Vẽ số lên tọa độ x, y
	void DrawNumber(int num, int x, int y, int size = 50)
	{
		stringstream ss;
		ss << num;
		DrawString(ss.str().data(), x, y, size);
	}

	// Dùng để fade màu lên màn hình, với 5 tham số: shader cần dùng, alpha, red, green, blue
	static void DrawFade(GLuint shad, float alpha = 1.0, float r = 0.0, float g = 0.0, float b = 0.0)
	{
		glDisable(GL_DEPTH_TEST);
		GLuint shaderID = shad;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(shaderID);
		
		GLuint uAloc = glGetUniformLocation(shaderID, "uAlpha");
		glUniform1f(uAloc, alpha);

		GLuint uCloc = glGetUniformLocation(shaderID, "uColor");
		glUniform3f(uCloc, r, g, b);

		Vector3 vdat[4];
		float buff[] = {
			-1, -1, 0.0,
			-1,  1, 0.0,
			 1,  1, 0.0,
			 1, -1, 0.0
		};
		memcpy(&vdat, buff, sizeof(buff));
		GLuint posLoc = glGetAttribLocation(shaderID, "a_posL");
		if(posLoc != -1)
		{
			glEnableVertexAttribArray(posLoc);
			glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), vdat);
		}
		int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, indices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Chuyển đổi và kiểm tra tọa độ chuột nằm trong hình chữ nhật 

	static Vector2 ScreenPosToTex(int x, int y, int texSize)
	{
		float tx = (float)x / (float)texSize;
		float ty = (float)y / (float)texSize;
		return Vector2(tx, ty);
	}

	// Convert tọa độ chuột sang tọa độ OpenGL
	static Vector2 ScreenPosToPos(int Sx, int Sy)
	{
		float Px = ((float)Sx/((float)SCREEN_W/2) - 1.0f);
		float Py = ((float)-Sy/((float)SCREEN_H/2) + 1.0f);
		return Vector2(Px, Py);
	}
	// Convert tọa độ OpenGL thành tọa độ 2D trên màn hình (tọa độ chuột)
	static Vector2 PosToScreenPos(float Px, float Py)
	{
		int Sx = SCREEN_W/2 + (SCREEN_W/2)*Px;
		int Sy = SCREEN_H/2 - (SCREEN_H/2)*Py;
		return Vector2(Sx, Sy);
	}
	// Kiểm tra tọa độ chuột có nằm trong một hình trong OpenGL
	static bool IsInRect(int x, int y, float xrect, float yrect, float wrect, float hrect)
	{
		Vector2 sp = ScreenPosToPos(x, y);
		/*LOGI("\nScren Pos: %d : %d\n", x, y);
		LOGI("\nConverted Pos: %f : %f\n", sp.x, sp.y);*/
		return ( xrect <= sp.x && sp.x <= (xrect + wrect) ) && ( yrect >= sp.y && sp.y >= (yrect - hrect) );
	}
	//Kiểm tra tọa độ có nằm trong hình chữ nhật không, hình chữ nhật này sử dụng tọa độ của màn hình
	static bool IsInRectScreenPos(int x, int y, int xrect, int yrect, int wrect, int hrect)
	{
		return (xrect <= x && x <= xrect + wrect) && ( yrect <= y && y <= yrect + hrect);
	}
	static bool IsInRect(int x, int y, Rectangle r)
	{
		Vector2 sp = ScreenPosToPos(x, y);
		/*LOGI("\nScren Pos: %d : %d\n", x, y);
		LOGI("\nConverted Pos: %f : %f\n", sp.x, sp.y);*/
		return ( r.x <= sp.x && sp.x <= (r.x + r.w) ) && ( r.y >= sp.y && sp.y >= (r.y - r.h) );
	}

	// Hàm tạo một hình chữ nhật trong vertex buffer
	static Rectangle CreateRect(int ix, int iy, int iw, int ih)
	{
		Vector2 stPos = ScreenPosToPos(ix, iy);
		Vector2 enPos = ScreenPosToPos(ix + iw, iy + ih);
		float x, y, w, h;
		x = stPos.x; y = stPos.y;
		w = abs(enPos.x - stPos.x); h = abs(enPos.y - stPos.y);

		Vertex verticesData[4];

		verticesData[0].pos.x =  x;  verticesData[0].pos.y =  y;  verticesData[0].pos.z =  0.0f;
		verticesData[0].tex.x =	 0; verticesData[0].tex.y = 1; 
		verticesData[1].pos.x =  x;  verticesData[1].pos.y = y-h;  verticesData[1].pos.z =  0.0f;
		verticesData[1].tex.x = 0; verticesData[1].tex.y = 0; 
		verticesData[2].pos.x =  x+w;  verticesData[2].pos.y = y-h;  verticesData[2].pos.z =  0.0f;
		verticesData[2].tex.x = 1; verticesData[2].tex.y = 0; 
		verticesData[3].pos.x =  x+w;  verticesData[3].pos.y =  y;  verticesData[3].pos.z =  0.0f;
		verticesData[3].tex.x = 1; verticesData[3].tex.y = 1; 

		GLuint vboId;
		//buffer object
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);	

		Rectangle r;
		r.x = x; r.y = y; r.w = w; r.h = h; r.id = vboId;

		return r;
	}
	static Rectangle CreateRect(int ix, int iy, int iw, int ih, int tx, int ty, int tw, int th)
	{
		Vector2 stPos = ScreenPosToPos(ix, iy);
		Vector2 enPos = ScreenPosToPos(ix + iw, iy + ih);

		Vector2 tPos = ScreenPosToTex(tx, ty, 1024);
		Vector2 sPos = ScreenPosToTex(tw, th, 1024);

		float x, y, w, h, xx, yy, ww, hh;
		x = stPos.x; y = stPos.y;
		w = abs(enPos.x - stPos.x); h = abs(enPos.y - stPos.y);

		Vertex verticesData[4];

		verticesData[0].pos.x =  x;  verticesData[0].pos.y =  y;  verticesData[0].pos.z =  0.0f;
		verticesData[0].tex.x =	 tPos.x; verticesData[0].tex.y = tPos.y; 
		verticesData[1].pos.x =  x;  verticesData[1].pos.y = y-h;  verticesData[1].pos.z =  0.0f;
		verticesData[1].tex.x =  tPos.x; verticesData[1].tex.y = tPos.y + sPos.y; 
		verticesData[2].pos.x =  x+w;  verticesData[2].pos.y = y-h;  verticesData[2].pos.z =  0.0f;
		verticesData[2].tex.x =  tPos.x + sPos.x; verticesData[2].tex.y = tPos.y + sPos.y; 
		verticesData[3].pos.x =  x+w;  verticesData[3].pos.y =  y;  verticesData[3].pos.z =  0.0f;
		verticesData[3].tex.x =  tPos.x + sPos.x; verticesData[3].tex.y = tPos.y; 

		GLuint vboId;
		//buffer object
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);	

		Rectangle r;
		r.x = x; r.y = y; r.w = w; r.h = h; r.id = vboId;

		return r;
	}

	static Rectangle CreateRect(float x, float y, float w, float h)
	{
		Vertex verticesData[4];

		verticesData[0].pos.x =  x;  verticesData[0].pos.y =  y;  verticesData[0].pos.z =  0.0f;
		verticesData[0].tex.x =	 0; verticesData[0].tex.y = 1; 
		verticesData[1].pos.x =  x;  verticesData[1].pos.y = y-h;  verticesData[1].pos.z =  0.0f;
		verticesData[1].tex.x = 0; verticesData[1].tex.y = 0; 
		verticesData[2].pos.x =  x+w;  verticesData[2].pos.y = y-h;  verticesData[2].pos.z =  0.0f;
		verticesData[2].tex.x = 1; verticesData[2].tex.y = 0; 
		verticesData[3].pos.x =  x+w;  verticesData[3].pos.y =  y;  verticesData[3].pos.z =  0.0f;
		verticesData[3].tex.x = 1; verticesData[3].tex.y = 1; 

		GLuint vboId;
		//buffer object
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);	

		Rectangle r;
		r.x = x; r.y = y; r.w = w; r.h = h; r.id = vboId;

		return r;
	}
	static Rectangle CreateRect(float x, float y, float w, float h, Vector2* tex)
	{
		Vertex verticesData[4];

		verticesData[0].pos.x =  x;  verticesData[0].pos.y =  y;  verticesData[0].pos.z =  0;
		verticesData[0].tex.x =	 tex[0].x; verticesData[0].tex.y = tex[0].y; 
		verticesData[1].pos.x =  x;  verticesData[1].pos.y = y-h;  verticesData[1].pos.z =  0;
		verticesData[1].tex.x =	 tex[1].x; verticesData[1].tex.y = tex[1].y; 
		verticesData[2].pos.x =  x+w;  verticesData[2].pos.y = y-h;  verticesData[2].pos.z =  0;
		verticesData[2].tex.x =	 tex[2].x; verticesData[2].tex.y = tex[2].y; 
		verticesData[3].pos.x =  x+w;  verticesData[3].pos.y =  y;  verticesData[3].pos.z =  0;
		verticesData[3].tex.x =	 tex[3].x; verticesData[3].tex.y = tex[3].y; 

		GLuint vboId;
		//buffer object
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);	

		Rectangle r;
		r.x = x; r.y = y; r.w = w; r.h = h; r.id = vboId;

		return r;
	}
	static Rectangle CreateRect(float x, float y, float z, float w, float h, Vector2* tex)
	{
		Vertex verticesData[4];

		verticesData[0].pos.x =  x;  verticesData[0].pos.y =  y;  verticesData[0].pos.z =  z;
		verticesData[0].tex.x =	 tex[0].x; verticesData[0].tex.y = tex[0].y; 
		verticesData[1].pos.x =  x;  verticesData[1].pos.y = y-h;  verticesData[1].pos.z =  z;
		verticesData[1].tex.x =	 tex[1].x; verticesData[1].tex.y = tex[1].y; 
		verticesData[2].pos.x =  x+w;  verticesData[2].pos.y = y-h;  verticesData[2].pos.z =  z;
		verticesData[2].tex.x =	 tex[2].x; verticesData[2].tex.y = tex[2].y; 
		verticesData[3].pos.x =  x+w;  verticesData[3].pos.y =  y;  verticesData[3].pos.z =  z;
		verticesData[3].tex.x =	 tex[3].x; verticesData[3].tex.y = tex[3].y; 

		GLuint vboId;
		//buffer object
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);	

		Rectangle r;
		r.x = x; r.y = y; r.w = w; r.h = h; r.id = vboId;

		return r;
	}

	// Hàm load một texture vào texture buffer
	static GLint CreateTexture(const char* fileName, const char* tiling = "REPEAT")
	{
		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		int imgWidth, imgHeight, imgBpp;
		char* imgData = LoadTGA(FileSystem::GetInstance()->GetPath(fileName).c_str(), &imgWidth, &imgHeight, &imgBpp);
		glTexImage2D(GL_TEXTURE_2D, 0, (imgBpp==32)?GL_RGBA:GL_RGB, imgWidth, imgHeight, 0, (imgBpp==32)?GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE, imgData);

		if (tiling == "CLAMP")
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		}	
	
		// not using mipmap
		/*glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);*/
		// using mipmap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		delete[] imgData;

		return texID;
	}

	// Hàm load shader
	static Shaders* CreateShader(const char* vs, const char* fs)
	{
		Shaders* s = new Shaders;
		s->Init(vs, fs);
		return s;
	}
	
	// Hàm vẽ một hình chữ nhật ra màn hình
	static void DrawRect(GLuint vboID, GLuint shaderID = -1, GLuint texID = -1)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (shaderID != -1)
			glUseProgram(shaderID);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		if (shaderID != -1)
		{
			GLint pos = glGetAttribLocation(shaderID, "a_position");
			if(pos != -1)
			{
				glEnableVertexAttribArray(pos);
				glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			}
			if (texID != -1)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texID);
				GLuint uTexLoc = glGetUniformLocation(shaderID, "u_texture");
				glUniform1i(uTexLoc, 0);
				GLint tex = glGetAttribLocation(shaderID, "a_uv");
				if (tex != -1)
				{
					glEnableVertexAttribArray(tex);
					glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
				}
			}
		}

		int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, indices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};

