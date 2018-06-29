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

#include "levelSelect.h"

static void logic(void);
static void draw(void);
static void initLevelRects(void);

static Atlas *levelSelectRect;
static Atlas *padlock;
static Atlas *levelStarMissing;
static Atlas *levelStarFound;
static Background background;
static LevelRect levelRect[MAX_LEVELS];
static const int MIN_Y = 60;
static int MAX_Y;
static int y;

void initLevelSelect(void)
{
	initGLRectangle(&background.rect, SCREEN_WIDTH, SCREEN_HEIGHT);
	background.rect.texture = loadTexture("gfx/backgrounds/background.jpg")->texture;
	background.r = background.g = background.b = 1.0;
	
	levelSelectRect = getImageFromAtlas("gfx/levelSelect/levelSelectRectangle.png", 1);
	padlock = getImageFromAtlas("gfx/levelSelect/padlock.png", 1);
	levelStarMissing = getImageFromAtlas("gfx/levelSelect/levelStarMissing.png", 1);
	levelStarFound = getImageFromAtlas("gfx/levelSelect/levelStarFound.png", 1);
	
	y = MIN_Y;
	
	initLevelRects();
	
	app.delegate.logic = logic;
	app.delegate.draw = draw;
}

static void initLevelRects(void)
{
	int i, x, y;
	LevelRect *l;
	
	y = MIN_Y + 120;
	x = 75;
	
	for (i = 0 ; i < MAX_LEVELS ; i++)
	{
		l = &levelRect[i];
		
		l->x = x;
		l->y = y;
		l->r = 0.5f + (float) ((randF() * 50) / 100);
		l->g = 0.5f + (float) ((randF() * 50) / 100);
		l->b = 0.5f + (float) ((randF() * 50) / 100);
		l->levelNum = i + 1;
		l->available = (i <= game.levelsCompleted);
		l->hasStar = game.starsAvailable[i];
		l->hasFoundStar = game.starsFound[i];
		
		x += 225;

		if (x > 600)
		{
			x = 75;
			y += 225;
		}

		MAX_Y = -y;
	}
	
	MAX_Y += SCREEN_HEIGHT / 2;
}

static void logic(void)
{
}

static void draw(void)
{
	int i;
	LevelRect *l;
	
	drawBackground(&background);
	
	for (i = 0 ; i < MAX_LEVELS ; i++)
	{
		l = &levelRect[i];
		
		if (l->available)
		{
			setGLRectangleBatchColor(l->r, l->g, l->b, 1);
		}
		else
		{
			setGLRectangleBatchColor(1, 1, 1, 1);
		}

		drawGLRectangleBatch(&levelSelectRect->rect, l->x, l->y + y, 0);
		
		setGLRectangleBatchColor(1, 1, 1, 1);
		
		if (!l->available)
		{
			drawGLRectangleBatch(&padlock->rect, l->x + 80, l->y + y + 80, 0);
		}

		if (l->hasFoundStar)
		{
			drawGLRectangleBatch(&levelStarFound->rect, l->x + levelSelectRect->rect.w / 2, (int) (y + l->y + levelSelectRect->rect.h + 32), 1);
		}
		else if (l->hasStar)
		{
			drawGLRectangleBatch(&levelStarMissing->rect, l->x + levelSelectRect->rect.w / 2, (int) (y + l->y + levelSelectRect->rect.h + 32), 1);
		}
	}
	
	useFont("cardigan48");
	
	for (i = 0 ; i < MAX_LEVELS ; i++)
	{
		l = &levelRect[i];
		
		drawText(l->x + 58, l->y + y + 28, TA_CENTER, "%d", i + 1);
	}
	
	drawText(SCREEN_WIDTH / 2, (int) y, TA_CENTER, "Level Select");
	
	useFont("cardigan40");
	drawText(SCREEN_WIDTH / 2, (int) y + 70, TA_CENTER, "%d / %d", game.levelsCompleted, MAX_LEVELS);
}
