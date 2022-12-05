#include "RenderFunctions.h"
#include "GLIncludes.h"
#include <vector>
#include <iostream>
#include <cmath>
#include "lodepng.h"

#define PI 3.14159265358979323846

void RedTriangle()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0, -0.5);
	glEnd();
}

void ColoredTriangle()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);

	glColor3f(1, 0, 0);
	glVertex2f(-0.5, 0.5);

	glColor3f(0, 1, 0);
	glVertex2f(0.5, 0.5);

	glColor3f(0, 0, 1);
	glVertex2f(0, -0.5);

	glEnd();
}

int LoadTexture()
{
	std::vector<uint8_t> imageData;
	uint32_t width, height;
	uint32_t error = lodepng::decode(imageData, width, height, "Bricks.png");
	if (error != 0)
	{
		std::cerr << "Error loading texture: " << lodepng_error_text(error) << std::endl;
		return 0;
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return textureId;
}

void TexturedQuad(int textureId)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glColor3f(1, 1, 1);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(-0.5, 0.5);

	glTexCoord2f(1, 0);
	glVertex2f(0.5, 0.5);

	glTexCoord2f(1, 1);
	glVertex2f(0.5, -0.5);

	glTexCoord2f(0, 1);
	glVertex2f(-0.5, -0.5);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void RotatedTriangle(float time)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(time * 45, 0, 0, 1);

	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex2f(-0.5, -0.5);
	glColor3f(0, 1, 0);
	glVertex2f(-0.5, 0.5);
	glColor3f(0, 0, 1);
	glVertex2f(0.5, 0);
	glEnd();
}

void LoadPerspective(int fovY, float near, float far, float windowWidth, float windowHeight)
{
	/*float fov = fovY * PI / 180.0;
	float halfHeight = tan(fov / 2) * near;
	float halfWidth = halfHeight / (float)windowHeight * windowWidth;

	glLoadIdentity();
	glFrustum(-halfHeight, halfHeight, -halfHeight, halfHeight, near, far);
	glOrtho(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);*/
	
	float fovX = fovY / windowHeight * windowWidth;
	float S = 1.0 / tan(fovY / 2 * PI / 180);
	float mat[] = {
		S, 0, 0, 0,
		0, S, 0, 0,
		0, 0, -far/(far-near), -1,
		0, 0, -far * near / (far - near), 0
	};
	glLoadMatrixf(mat);
	float mat2[] = {
		windowHeight / windowWidth, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	glMultMatrixf(mat2);
	float h = 1;
	float w = h / windowHeight * windowWidth;
	//glOrtho(-w, w, -h, h, near, far);
}

void Cube(float time, float windowWidth, float windowHeight)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0, 0, 0, 1);
	glClearDepth(GL_DEPTH_CLEAR_VALUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	LoadPerspective(90, 0.1, 100, windowWidth, windowHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -20);
	glRotatef(time * 45, 0, 1, 0);
	glRotatef(time * 90, 1, 0, 0);

	bool colored = true;
	float size = 5;

	if (!colored) glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	// Front
	if(colored) glColor3f(1, 0, 0);
	glVertex3f(size, size, -size);
	glVertex3f(-size, size, -size);
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);
	// Back
	if(colored) glColor3f(1, 1, 0);
	glVertex3f(size, size, size);
	glVertex3f(-size, size, size);
	glVertex3f(-size, -size, size);
	glVertex3f(size, -size, size);
	// Side size
	if(colored) glColor3f(0, 1, 0);
	glVertex3f(size, size, size);
	glVertex3f(size, -size, size);
	glVertex3f(size, -size, -size);
	glVertex3f(size, size, -size);
	// Side 2
	if(colored) glColor3f(0, 1, 1);
	glVertex3f(-size, size, size);
	glVertex3f(-size, -size, size);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, size, -size);
	// Top
	if(colored) glColor3f(0, 0, 1);
	glVertex3f(size, size, size);
	glVertex3f(-size, size, size);
	glVertex3f(-size, size, -size);
	glVertex3f(size, size, -size);
	// Bottom
	if(colored) glColor3f(size, 0, 1);
	glVertex3f(size, -size, size);
	glVertex3f(-size, -size, size);
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);
	glEnd();
}