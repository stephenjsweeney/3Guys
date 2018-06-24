/*
Copyright (C) 2018 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "glRectangleBatch.h"

static void initIndices(void);
void flushGLRectangleBatch(void);
static void rotateVertexes(int w, int h);
static void swap(float values[], int a, int b);
static void flipImage(void);

void initGLRectangleBatch(void)
{
	initIndices();
	
	glRectangleBatch.currentTexture = -1;
	glRectangleBatch.numAddedRects = 0;
	glRectangleBatch.vIndex = glRectangleBatch.cIndex = glRectangleBatch.tIndex = 0;
	glRectangleBatch.flipHorizontal = glRectangleBatch.flipVertical = 0;
	glRectangleBatch.rotate = 0;
	glRectangleBatch.scale = 1.0f;
	glRectangleBatch.angle = 0;
	glRectangleBatch.flushes = 0;
	
	memset(&glRectangleBatch.color, 1, sizeof(int) * 4);
}

static void initIndices(void)
{
	int i;
	int n = 0;

	for (i = 0; i < MAX_RECTS * 6; i += 6)
	{
		glRectangleBatch.indices[i] = (0 + n);
		glRectangleBatch.indices[i + 1] = (1 + n);
		glRectangleBatch.indices[i + 2] = (2 + n);
		glRectangleBatch.indices[i + 3] = (1 + n);
		glRectangleBatch.indices[i + 4] = (2 + n);
		glRectangleBatch.indices[i + 5] = (3 + n);

		n += 4;
	}
}

void drawGLRectangleBatch(GLRectangle *rect, int x, int y, int center)
{
	int i, m;
	
	if (center)
	{
		x -= rect->w / 2;
		y -= rect->h / 2;
	}
	
	if (rect->texture != glRectangleBatch.currentTexture)
	{
		flushGLRectangleBatch();

		glRectangleBatch.currentTexture = rect->texture;
	}
	
	/* append the rect's vertices points */
	for (i = 0; i < MAX_VERTS; i++)
	{
		glRectangleBatch.vertices[glRectangleBatch.vIndex] = rect->vertices[i];

		m = i % 3;

		/* add the x coordinate, as needed */
		if (m == 0)
		{
			glRectangleBatch.vertices[glRectangleBatch.vIndex] *= glRectangleBatch.scale;

			glRectangleBatch.vertices[glRectangleBatch.vIndex] += x;
		}
		/* add the y coordinate, as needed */
		else if (m == 1)
		{
			glRectangleBatch.vertices[glRectangleBatch.vIndex] *= glRectangleBatch.scale;

			glRectangleBatch.vertices[glRectangleBatch.vIndex] += y;
		}

		glRectangleBatch.vIndex++;
	}

	if (glRectangleBatch.rotate)
	{
		rotateVertexes(rect->w, rect->h);
	}

	/* append the rect's texture coordinates */
	for (i = 0; i < MAX_TEX_CORDS; i++)
	{
		glRectangleBatch.textureCords[glRectangleBatch.tIndex++] = rect->textureCords[i];
	}

	if (glRectangleBatch.flipHorizontal || glRectangleBatch.flipVertical)
	{
		flipImage();
	}

	/* copy the current color to the four vertex points for our rectangle */
	for (i = 0; i < 16; i += 4)
	{
		memcpy(&glRectangleBatch.colors[glRectangleBatch.cIndex + i], glRectangleBatch.color, sizeof(int) * 4);
	}

	glRectangleBatch.cIndex += 16;

	if (++glRectangleBatch.numAddedRects >= MAX_RECTS)
	{
		flushGLRectangleBatch();
	}
}

static void rotateVertexes(int w, int h)
{
	int v, i;
	float x, y, cx, cy;
	double c, s;
	
	i = glRectangleBatch.vIndex - MAX_VERTS;

	w /= 2;
	h /= 2;

	cx = glRectangleBatch.vertices[i] + w;
	cy = glRectangleBatch.vertices[i + 1] + h;

	s = sin(TO_RAIDANS(glRectangleBatch.angle));
	c = cos(TO_RAIDANS(glRectangleBatch.angle));

	for (v = 0; v < 12; v += 3)
	{
		x = glRectangleBatch.vertices[i + v] - cx;
		y = glRectangleBatch.vertices[i + v + 1] - cy;

		glRectangleBatch.vertices[i + v] = (float) ((x * c) - (y * s));
		glRectangleBatch.vertices[i + v + 1] = (float) ((x * s) + (y * c));

		glRectangleBatch.vertices[i + v] += cx;
		glRectangleBatch.vertices[i + v + 1] += cy;
	}
}

static void flipImage()
{
	int i = glRectangleBatch.tIndex - MAX_TEX_CORDS;

	if (glRectangleBatch.flipHorizontal)
	{
		swap(glRectangleBatch.textureCords, i + 0, i + 2);
		swap(glRectangleBatch.textureCords, i + 4, i + 6);
	}

	if (glRectangleBatch.flipVertical)
	{
		swap(glRectangleBatch.textureCords, i + 1, i + 3);
		swap(glRectangleBatch.textureCords, i + 5, i + 7);
	}
}

static void swap(float *values, int a, int b)
{
	float c = values[a];
	values[a] = values[b];
	values[b] = c;
}

void setGLRectangleBatchColor(float r, float g, float b, float a)
{
	glRectangleBatch.color[0] = r;
	glRectangleBatch.color[1] = g;
	glRectangleBatch.color[2] = b;
	glRectangleBatch.color[3] = a;
}

void flushGLRectangleBatch()
{
	if (glRectangleBatch.numAddedRects > 0)
	{
		glEnable(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D, glRectangleBatch.currentTexture);
		
		glColorPointer(4, GL_FLOAT, 0, glRectangleBatch.colors);
		
		glVertexPointer(3, GL_FLOAT, 0, glRectangleBatch.vertices);
		
		glTexCoordPointer(2, GL_FLOAT, 0, glRectangleBatch.textureCords);

		glDrawElements(GL_TRIANGLES, (glRectangleBatch.numAddedRects * 6), GL_UNSIGNED_INT, glRectangleBatch.indices);
		
		glRectangleBatch.flushes++;
	}

	memset(&glRectangleBatch.vertices, 0, sizeof(int) * MAX_RECTS * MAX_VERTS);
	
	glRectangleBatch.vIndex = glRectangleBatch.tIndex = glRectangleBatch.cIndex = 0;

	glRectangleBatch.numAddedRects = 0;
}
