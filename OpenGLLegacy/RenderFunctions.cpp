#include "RenderFunctions.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "lodepng.h"

#define PI 3.14159265358979323846

void RedTriangle()
{
	glClearColor(0, 0, 0, 1);		// Rücksetzfarbe auf Schwarz setzen
	glClear(GL_COLOR_BUFFER_BIT);	// Bildschirm leeren

	glColor3f(1, 0, 0);				// Farbe auf Rot setzen
	glBegin(GL_TRIANGLES);			// Dreieck beginnen
	glVertex2f(-0.5, 0.5);			// Ersten Eckpunkt senden
	glVertex2f(0.5, 0.5);			// Zweiten Eckpunkt senden
	glVertex2f(0, -0.5);			// Dritten Eckpunkt senden
	glEnd();						// Dreieck abschließen
}

void ColoredTriangle()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);

	glColor3f(1, 0, 0);				// Farbe auf Rot setzen
	glVertex2f(-0.5, 0.5);

	glColor3f(0, 1, 0);				// Farbe auf Grün setzen
	glVertex2f(0.5, 0.5);

	glColor3f(0, 0, 1);				// Farbe auf Blau setzen
	glVertex2f(0, -0.5);

	glEnd();
}

int LoadTexture()
{
	// Bilddatei dekodieren
	std::vector<uint8_t> imageData;
	uint32_t width, height;
	uint32_t error = lodepng::decode(imageData, width, height, "Bricks.png");
	if (error != 0)
	{
		std::cerr << "Error loading texture: " << lodepng_error_text(error) << std::endl;
		return 0;
	}

	GLuint textureId;
	// OpenGL Textur erzeugen
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	// Textur mit Daten befüllen
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());

	// Textur Filter aktivieren
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return textureId;
}

void TexturedQuad(int textureId)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);					// Texturen aktivieren
	glBindTexture(GL_TEXTURE_2D, textureId);	// Textur zum rendern auswählen
	glColor3f(1, 1, 1);			// Die Farbe wird mit den Farben der Textur multipliziert
								// daher sollte diese auf (1, 1, 1) gesetzt sein

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);			// Obere linke Ecke der Textur auswählen
	glVertex2f(-0.5, 0.5);

	glTexCoord2f(1, 0);			// Obere rechte Ecke der Textur auswählen
	glVertex2f(0.5, 0.5);

	glTexCoord2f(1, 1);
	glVertex2f(0.5, -0.5);		// Untere rechte Ecke der Textur auswählen

	glTexCoord2f(0, 1);
	glVertex2f(-0.5, -0.5);		// Untere linke Ecke der Textur auswählen

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void RotatedTriangle(float time)
{
	// time: Die Zeit seit dem Programmstart in Sekunden

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);		// Die Modelview Matrix bearbeiten
	glLoadIdentity();				// Modelview Matrix auf Einheitsmatrix zurücksetzen
	glRotatef(time * 45, 0, 0, 1);	// Modelview Matrix um 45° pro Sekunde um die z-Achse drehen

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
	// In mat1 wird eine Perspektivmatrix erzeugt
	// In mat2 wird eine Matrix erzeugt, um das Seitenverhältnis des Fensters auszugleichen
	// Diese beiden Matrizen werden miteinander multipliziert um die Perspektiv Matrix zu bilden

	float S = 1.0 / tan(fovY / 2 * PI / 180);
	float mat1[] = {
		S, 0, 0, 0,
		0, S, 0, 0,
		0, 0, -far/(far-near), -1,
		0, 0, -far * near / (far - near), 0
	};
	glLoadMatrixf(mat1);
	float mat2[] = {
		windowHeight / windowWidth, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	glMultMatrixf(mat2);
}

void Cube(float time, float windowWidth, float windowHeight)
{
	glEnable(GL_DEPTH_TEST);			// Tiefen-Test aktivieren

	glClearColor(0, 0, 0, 1);
	glClearDepth(GL_DEPTH_CLEAR_VALUE);	// Rücksetztiefe einstellen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Farbwerte und Tiefeninformationen
														// zurücksetzen

	glMatrixMode(GL_PROJECTION);	// Projektions Matrix bearbeiten
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
	// Side 1
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