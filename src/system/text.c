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

#include "text.h"

static void initFont(char *name, char *filename);
static void drawWord(char *word, int *x, int *y, int startX);
static void applyWordWrap(char *word, int *x, int *y, int startX);
void calcTextDimensions(char *text, int size, int *w, int *h);
void drawText(int x, int y, int align, int size, const char *format, ...);
void useFont(char *name);

static SDL_Color white = {255, 255, 255, 255};
static int textWidth = 0;
static char drawTextBuffer[1024];
static char shadowTextBuffer[1024];
static Font fontHead;
static Font *fontTail;
static Font *activeFont = NULL;

void initFonts(void)
{
	memset(&fontHead, 0, sizeof(Font));
	fontTail = &fontHead;
	
	initFont("cardigan", getFileLocation("fonts/cardigan titling rg.ttf"));
	
	useFont("cardigan");
}

static void initFont(char *name, char *filename)
{
	uint32_t texture;
	TTF_Font *font;
	Font *f;
	SDL_Surface *surface, *text;
	SDL_Rect dest;
	int i;
	char c[2];
	float x1, y1, x2, y2;
		
	surface = SDL_CreateRGBSurface(0, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 32, 0, 0, 0, 0xff);
	
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
	
	font = TTF_OpenFont(filename, FONT_SIZE);
	
	f = malloc(sizeof(Font));
	memset(f, 0, sizeof(Font));
	
	dest.x = dest.y = 0;
	
	for (i = ' ' ; i <= 'z' ; i++)
	{
		memset(c, 0, 2);
		
		sprintf(c, "%c", i);
		
		text = TTF_RenderUTF8_Blended(font, c, white);
		
		TTF_SizeText(font, c, &dest.w, &dest.h);
		
		if (dest.x + dest.w >= FONT_TEXTURE_SIZE)
		{
			dest.x = 0;
			
			dest.y += dest.h + 1;
		}
		
		SDL_BlitSurface(text, NULL, surface, &dest);
		
		x1 = dest.x;
		y1 = dest.y;
		x2 = (dest.x + dest.w);
		y2 = (dest.y + dest.h);
		
		x1 /= FONT_TEXTURE_SIZE;
		y1 /= FONT_TEXTURE_SIZE;
		x2 /= FONT_TEXTURE_SIZE;
		y2 /= FONT_TEXTURE_SIZE;
		
		setGLRectangleSize(&f->glyphs[i], dest.w, dest.h);
		
		setGLRectangleTextureCoords(&f->glyphs[i], x1, y1, x2, y2);
		
		SDL_FreeSurface(text);
		
		dest.x += dest.w;
	}
	
	TTF_CloseFont(font);
	
	texture = toTexture(surface, 1);
	
	for (i = 0 ; i < 128 ; i++)
	{
		f->glyphs[i].texture = texture;
	}
	
	strcpy(f->name, name);
	
	fontTail->next = f;
	fontTail = f;
}

void drawShadowText(int x, int y, int align, int size, const char *format, ...)
{
	va_list args;
	float color[4];
	
	memset(&shadowTextBuffer, '\0', sizeof(drawTextBuffer));

	va_start(args, format);
	vsprintf(shadowTextBuffer, format, args);
	va_end(args);
	
	memcpy(color, glRectangleBatch.color, sizeof(float) * 4);
	
	setGLRectangleBatchColor(0, 0, 0, 1);
	drawText(x + 4, y + 4, align, size, shadowTextBuffer);
	
	setGLRectangleBatchColor(color[0], color[1], color[2], color[3]);
	drawText(x, y, align, size, shadowTextBuffer);
}

void drawText(int x, int y, int align, int size, const char *format, ...)
{
	int i, startX, n, w, h;
	char word[128];
	va_list args;

	if (activeFont)
	{
		memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

		va_start(args, format);
		vsprintf(drawTextBuffer, format, args);
		va_end(args);
		
		glRectangleBatch.scale = size / (FONT_SIZE * 1.0f);
		
		startX = x;
		
		memset(word, 0, 128);
		
		n = 0;
		
		calcTextDimensions(drawTextBuffer, size, &w, &h);
		
		if (align == TA_RIGHT)
		{
			x -= w;
		}
		else if (align == TA_CENTER)
		{
			x -= (w / 2);
		}
		
		for (i = 0 ; i < strlen(drawTextBuffer) ; i++)
		{
			word[n++] = drawTextBuffer[i];
			
			if (drawTextBuffer[i] == ' ')
			{
				drawWord(word, &x, &y, startX);
				
				memset(word, 0, 128);
				
				n = 0;
			}
		}
		
		drawWord(word, &x, &y, startX);
		
		glRectangleBatch.scale = 1.0f;
	}
}

static void drawWord(char *word, int *x, int *y, int startX)
{
	int i, c;
	
	if (textWidth > 0)
	{
		applyWordWrap(word, x, y, startX);
	}
	
	for (i = 0 ; i < strlen(word) ; i++)
	{
		c = word[i];
		
		drawGLRectangleBatch(&activeFont->glyphs[c], *x, *y, 0);
		
		*x += activeFont->glyphs[c].w * glRectangleBatch.scale;
	}
}

static void applyWordWrap(char *word, int *x, int *y, int startX)
{
	int i, w, c;
	
	w = 0;
	
	for (i = 0 ; i < strlen(word) ; i++)
	{
		c = word[i];
		
		w += activeFont->glyphs[c].w * glRectangleBatch.scale;
		
		if (*x + w > textWidth)
		{
			*x = startX;
			
			*y += activeFont->glyphs[c].h * glRectangleBatch.scale;
			
			return;
		}
	}
}

void useFont(char *name)
{
	Font *f;
	
	for (f = fontHead.next ; f != NULL ; f = f->next)
	{
		if (strcmp(f->name, name) == 0)
		{
			activeFont = f;
			return;
		}
	}
}

void calcTextDimensions(char *text, int size, int *w, int *h)
{
	int i, c;
	float scale;
	
	scale = size / (FONT_SIZE * 1.0f);
	
	*w = 0;
	*h = 0;
	
	for (i = 0 ; i < strlen(text) ; i++)
	{
		c = text[i];
		
		*w += activeFont->glyphs[c].w * scale;
		*h = MAX(activeFont->glyphs[c].h * scale, *h);
	}
}

void setTextWidth(int width)
{
	textWidth = width;
}
