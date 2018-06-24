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

#include "glRectangle.h"

void initGLRectangle(GLRectangle *rect, int width, int height)
{
	memset(rect, 0, sizeof(GLRectangle));
	
	rect->w = width;
	rect->h = height;

	rect->vertices[3] = width;
	rect->vertices[7] = height;
	rect->vertices[9] = width;
	rect->vertices[10] = height;
	
	rect->textureCords[2] = 1.0f;
	rect->textureCords[5] = 1.0f;
	rect->textureCords[6] = 1.0f;
	rect->textureCords[7] = 1.0f;
}

void setGLRectangleSize(GLRectangle *rect, int w, int h)
{
	rect->w = w;
	rect->h = h;

	rect->vertices[3] = w;
	rect->vertices[7] = h;
	rect->vertices[9] = w;
	rect->vertices[10] = h;
}

void setGLRectangleTextureCoords(GLRectangle *rect, float x1, float y1, float x2, float y2)
{
	/* Top left */
	rect->textureCords[0] = x1;
	rect->textureCords[1] = y1;

	/* Top right */
	rect->textureCords[2] = x2;
	rect->textureCords[3] = y1;

	/* Bottom left */
	rect->textureCords[4] = x1;
	rect->textureCords[5] = y2;

	/* Bottom right */
	rect->textureCords[6] = x2;
	rect->textureCords[7] = y2;
}
