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
static void initLevelRects(int n);
static void doLevelSelect(void);
static void prev(void);
static void next(void);
static void back(void);
static void countStars(void);

static Atlas *levelSelectRect;
static Atlas *padlock;
static Atlas *levelStarMissing;
static Atlas *levelStarFound;
static Background background;
static LevelRect levelRect[MAX_LEVEL_PER_PAGE];
static int page = -1;
static int starsFound;
static int starsAvailable;

void initLevelSelect(void)
{
	initGLRectangle(&background.rect, SCREEN_WIDTH, SCREEN_HEIGHT);
	background.rect.texture = loadTexture("gfx/backgrounds/background.jpg")->texture;
	background.r = background.g = background.b = 1.0;
	
	levelSelectRect = getImageFromAtlas("gfx/levelSelect/levelSelectRectangle.png", 1);
	padlock = getImageFromAtlas("gfx/levelSelect/padlock.png", 1);
	levelStarMissing = getImageFromAtlas("gfx/levelSelect/levelStarMissing.png", 1);
	levelStarFound = getImageFromAtlas("gfx/levelSelect/levelStarFound.png", 1);
	
	if (page == -1)
	{
		page = game.levelsCompleted / MAX_LEVEL_PER_PAGE;
	}
	
	initLevelRects(page);
	
	initWipe(WIPE_FADE);
	
	showWidgetGroup("levelSelect");
	
	getWidget("prev", "levelSelect")->action = prev;
	getWidget("next", "levelSelect")->action = next;
	getWidget("back", "levelSelect")->action = back;
	
	countStars();
	
	getWidget("prev", "levelSelect")->disabled = page == 0;
	getWidget("next", "levelSelect")->disabled = page >= (MAX_LEVELS / MAX_LEVEL_PER_PAGE);
	
	app.delegate.logic = logic;
	app.delegate.draw = draw;
}

static void initLevelRects(int page)
{
	int i, x, y, n;
	LevelRect *l;
	
	x = 75;
	y = 260;
	
	for (i = 0 ; i < MAX_LEVEL_PER_PAGE ; i++)
	{
		n = i + (MAX_LEVEL_PER_PAGE * page);
		
		l = &levelRect[i];
		
		l->x = x;
		l->y = y;
		l->levelNum = n + 1;
		l->available = (n <= game.levelsCompleted);
		l->hasStar = game.starsAvailable[n + 1];
		l->hasFoundStar = game.starsFound[n + 1];
		
		x += 225;

		if (x > 600)
		{
			x = 75;
			y += 225;
		}
	}
}

static void countStars(void)
{
	int i;
	
	starsFound = starsAvailable = 0;
	
	for (i = 0 ; i < MAX_LEVELS ; i++)
	{
		if (game.starsAvailable[i] != 0)
		{
			starsAvailable++;
		}
		
		if (game.starsFound[i] != 0)
		{
			starsFound++;
		}
	}
}

static void logic(void)
{
	doWipe();
	
	doLevelSelect();
	
	doWidgets();
}

static void doLevelSelect(void)
{
	int i;
	LevelRect *l;
	
	if (app.mouse.button[SDL_BUTTON_LEFT])
	{
		for (i = 0 ; i < MAX_LEVEL_PER_PAGE ; i++)
		{
			l = &levelRect[i];
			
			if (collision(app.mouse.x / app.scaleX, app.mouse.y / app.scaleY, 1, 1, l->x, l->y, levelSelectRect->rect.w, levelSelectRect->rect.h))
			{
				if (l->available)
				{
					initLevel(l->levelNum);
				}
				else
				{
					playSound(SND_DENIED, 0);
				}
				
				app.mouse.button[SDL_BUTTON_LEFT] = 0;
			}
		}
	}
}

static void draw(void)
{
	int i;
	LevelRect *l;
	
	drawBackground(&background);
	
	for (i = 0 ; i < MAX_LEVEL_PER_PAGE ; i++)
	{
		l = &levelRect[i];
		
		if (l->levelNum <= MAX_LEVELS)
		{
			if (l->available)
			{
				if (l->levelNum <= game.levelsCompleted)
				{
					setGLRectangleBatchColor(0.5, 1.0, 0.5, 1);
				}
				else
				{
					setGLRectangleBatchColor(1.0, 1.0, 0.5, 1);
				}
			}
			else
			{
				setGLRectangleBatchColor(1, 1, 1, 1);
			}

			drawGLRectangleBatch(&levelSelectRect->rect, l->x, l->y, 0);
			
			setGLRectangleBatchColor(1, 1, 1, 1);
			
			if (!l->available)
			{
				drawGLRectangleBatch(&padlock->rect, l->x + 80, l->y + 80, 0);
			}

			if (l->hasFoundStar)
			{
				drawGLRectangleBatch(&levelStarFound->rect, l->x + levelSelectRect->rect.w / 2, (int) (l->y + levelSelectRect->rect.h + 32), 1);
			}
			else if (l->hasStar)
			{
				drawGLRectangleBatch(&levelStarMissing->rect, l->x + levelSelectRect->rect.w / 2, (int) (l->y + levelSelectRect->rect.h + 32), 1);
			}
		}
	}
	
	for (i = 0 ; i < MAX_LEVEL_PER_PAGE ; i++)
	{
		l = &levelRect[i];
		
		if (l->levelNum <= MAX_LEVELS)
		{
			drawShadowText(l->x + 58, l->y + 28, TA_CENTER, 48, "%d", l->levelNum);
		}
	}
	
	drawShadowText(SCREEN_WIDTH / 2, 40, TA_CENTER, 48, app.strings[ST_LEVEL_SELECT]);
	
	drawShadowText(50, 150, TA_LEFT, 32, app.strings[ST_LEVELS], game.levelsCompleted, MAX_LEVELS);
	drawShadowText(SCREEN_WIDTH - 50, 150, TA_RIGHT, 32, app.strings[ST_STARS], starsFound, starsAvailable);
	
	drawWidgets();
	
	drawWipe();
}

static void prev(void)
{
	page--;
	
	initLevelRects(page);
	
	getWidget("prev", "levelSelect")->disabled = page == 0;
	getWidget("next", "levelSelect")->disabled = 0;
	
	playSound(SND_BUTTON, 0);
}

static void next(void)
{
	page++;
	
	initLevelRects(page);
	
	getWidget("prev", "levelSelect")->disabled = 0;
	getWidget("next", "levelSelect")->disabled = page >= (MAX_LEVELS / MAX_LEVEL_PER_PAGE);
	
	playSound(SND_BUTTON, 0);
}

static void back(void)
{
	playSound(SND_SELECT, 1);
	
	initTitle();
}
