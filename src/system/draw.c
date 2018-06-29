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

#include "draw.h"

static void initColor(SDL_Color *c, int r, int g, int b);

static float rectVertices[12];
static float rectColors[16];
static Atlas *whiteSquare;

void initGraphics(void)
{
	initColor(&colors.red, 255, 0, 0);
	initColor(&colors.orange, 255, 128, 0);
	initColor(&colors.yellow, 255, 255, 0);
	initColor(&colors.green, 0, 255, 0);
	initColor(&colors.blue, 0, 0, 255);
	initColor(&colors.cyan, 0, 255, 255);
	initColor(&colors.purple, 255, 0, 255);
	initColor(&colors.white, 255, 255, 255);
	initColor(&colors.black, 0, 0, 0);
	initColor(&colors.lightGrey, 192, 192, 192);
	initColor(&colors.darkGrey, 128, 128, 128);
	
	whiteSquare = getImageFromAtlas("gfx/main/whiteSquare.png", 1);

	app.scaleX = app.config.winWidth;
	app.scaleX /= SCREEN_WIDTH;
	app.scaleY = app.config.winHeight;
	app.scaleY /= SCREEN_HEIGHT;
	
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Scaling: %.2f x %.2f", app.scaleX, app.scaleY);
}

void prepareScene(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	setGLRectangleBatchColor(1.0, 1.0, 1.0, 1.0);
}

void presentScene(void)
{
	if (dev.debug)
	{
		useFont("cardigan18");
		
		drawText(5, SCREEN_HEIGHT - 25, TA_LEFT, "DEBUG MODE");
		
		if (dev.showFPS)
		{
			drawText(SCREEN_WIDTH - 5, SCREEN_HEIGHT - 25, TA_RIGHT, "FPS: %d", dev.fps);
		}
	}
	
	flushGLRectangleBatch();
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	SDL_GL_SwapWindow(app.window);
}

void drawRect(int x, int y, int w, int h, float r, float g, float b, float a)
{
	int i;
	
	flushGLRectangleBatch();
	
	memset(&rectVertices, 0, sizeof(float) * 12);
	
	rectVertices[3] = w;
	rectVertices[6] = w;
	rectVertices[7] = h;
	rectVertices[10] = h;
	
	for (i = 0; i < 16; i += 4)
	{
		rectColors[i + 0] = r;
		rectColors[i + 1] = g;
		rectColors[i + 2] = b;
		rectColors[i + 3] = a;
	}

	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	{
		glTranslatef(x, y, 0.0f);
		glFrontFace(GL_CW);
		glVertexPointer(3, GL_FLOAT, 0, rectVertices);
		glColorPointer(4, GL_FLOAT, 0, rectColors);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}
	glPopMatrix();
}

void drawFilledRect(int x, int y, int w, int h, float r, float g, float b, float a)
{
	setGLRectangleSize(&whiteSquare->rect, w, h);
	
	setGLRectangleBatchColor(r, g, b, a);
	
	drawGLRectangleBatch(&whiteSquare->rect, x, y, 0);
}

void drawBackground(Background *background)
{
	glTranslatef(background->x, background->y, 0);
	
	glMatrixMode(GL_TEXTURE);

	glPushMatrix();
	{
		setGLRectangleBatchColor(background->r, background->g, background->b, 1.0);
		glTranslatef(background->tx, background->ty, 0);
		drawGLRectangleBatch(&background->rect, 0, 0, 0);
		flushGLRectangleBatch();
	}
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	
	glTranslatef(-background->x, -background->y, 0);
}

static void initColor(SDL_Color *c, int r, int g, int b)
{
	memset(c, 0, sizeof(SDL_Color));
	c->r = r;
	c->g = g;
	c->b = b;
	c->a = 255;
}
