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

static AtlasImage *whiteSquare = NULL;

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

	app.backBuffer = SDL_CreateTexture(app.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

	app.scaleX = SCREEN_WIDTH;
	app.scaleX /= app.config.winWidth;
	app.scaleY = SCREEN_HEIGHT;
	app.scaleY /= app.config.winHeight;
	
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Scaling: %.2f x %.2f", app.scaleX, app.scaleY);
}

void prepareScene(void)
{
	SDL_SetRenderTarget(app.renderer, app.backBuffer);
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
	SDL_RenderClear(app.renderer);
}

void presentScene(void)
{
	if (dev.debug)
	{
		drawText(5, 0, TEXT_ALIGN_LEFT, 18, "DEBUG MODE");
		
		if (dev.showFPS)
		{
			drawText(SCREEN_WIDTH - 5, 0, TEXT_ALIGN_RIGHT, 18, "FPS: %d", dev.fps);
		}
	}
	
	SDL_SetRenderTarget(app.renderer, NULL);
	SDL_RenderCopy(app.renderer, app.backBuffer, NULL, NULL);
	SDL_RenderPresent(app.renderer);
}

void blit(SDL_Texture *texture, int x, int y, int center)
{
	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	
	if (center)
	{
		dest.x -= dest.w / 2;
		dest.y -= dest.h / 2;
	}
	
	SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y, int center)
{
	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;
	dest.w = src->w;
	dest.h = src->h;
	
	if (center)
	{
		dest.x -= dest.w / 2;
		dest.y -= dest.h / 2;
	}
	
	SDL_RenderCopy(app.renderer, texture, src, &dest);
}

void blitAtlasImage(AtlasImage *atlasImage, int x, int y, int center)
{
	blitRect(atlasImage->texture, &atlasImage->rect, x, y, center);
}

void blitAtlasImageRotated(AtlasImage *atlasImage, int x, int y, int center, float angle)
{
	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;
	dest.w = atlasImage->rect.w;
	dest.h = atlasImage->rect.h;
	
	if (center)
	{
		dest.x -= dest.w / 2;
		dest.y -= dest.h / 2;
	}
	
	SDL_RenderCopyEx(app.renderer, atlasImage->texture, &atlasImage->rect, &dest, angle, NULL, SDL_FLIP_NONE);
}

void drawFilledRect(int x, int y, int w, int h, int r, int g, int b, int a)
{
	SDL_Rect dest;
	
	if (whiteSquare)
	{
		dest.x = x;
		dest.y = y;
		dest.w = w;
		dest.h = h;
		
		SDL_SetTextureColorMod(whiteSquare->texture, r, g, b);
		SDL_SetTextureAlphaMod(whiteSquare->texture, a);
		
		SDL_RenderCopy(app.renderer, whiteSquare->texture, &whiteSquare->rect, &dest);
	}
	
	SDL_SetTextureAlphaMod(whiteSquare->texture, 255);
}

void drawRect(int x, int y, int w, int h, int r, int g, int b, int a)
{
	// Top
	drawFilledRect(x, y, w, 1, r, g, b, a);
	
	// Bottom
	drawFilledRect(x, y + h, w, 1, r, g, b, a);
	
	// Left
	drawFilledRect(x, y, 1, h, r, g, b, a);
	
	// Right
	drawFilledRect(x + w, y, 1, h, r, g, b, a);
}

void drawBackground(Background *background)
{
	int sx, sy, w, h, x, y;
	
	SDL_QueryTexture(background->texture, NULL, NULL, &w, &h);
	
	sx = background->x > 0 ? background->x - w : background->x;
	sy = background->y > 0 ? background->y - h : background->y;
	
	SDL_SetTextureColorMod(background->texture, background->r, background->g, background->b);
	
	for (x = sx ; x < SCREEN_WIDTH ; x += w)
	{
		for (y = sy ; y < SCREEN_HEIGHT ; y += h)
		{
			blit(background->texture, x, y, 0);
		}
	}
}

static void initColor(SDL_Color *c, int r, int g, int b)
{
	memset(c, 0, sizeof(SDL_Color));
	c->r = r;
	c->g = g;
	c->b = b;
	c->a = 255;
}
