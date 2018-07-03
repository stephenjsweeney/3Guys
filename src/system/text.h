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

#include "stdlib.h"
#include "string.h"

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_opengl.h"

#include "../common.h"

#define FONT_SIZE			120
#define FONT_TEXTURE_SIZE	1024

extern void drawGLRectangleBatch(GLRectangle *rect, int x, int y, int center);
extern void setGLRectangleBatchColor(float r, float g, float b, float a);
extern void setGLRectangleSize(GLRectangle *rect, int w, int h);
extern void setGLRectangleTextureCoords(GLRectangle *rect, float x1, float y1, float x2, float y2);
extern uint32_t toTexture(SDL_Surface *surface, int destroySurface);

extern GLRectangleBatch glRectangleBatch;
