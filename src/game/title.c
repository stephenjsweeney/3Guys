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

#include "title.h"

static void logic(void);
static void draw(void);
static void postOptions(void);
static void initBouncers(void);
static void play(void);
static void options(void);
static void stats(void);
static void credits(void);
static void quit(void);

static Background background;
static Atlas *logo;
static Atlas *bouncerTypes[MAX_BOUNCER_TYPES];
static Bouncer bouncers[MAX_BOUNCERS];
static int isPlayingMusic = 0;

void initTitle(void)
{
	initGLRectangle(&background.rect, SCREEN_WIDTH, SCREEN_HEIGHT);
	background.rect.texture = loadTexture("gfx/backgrounds/background.jpg")->texture;
	background.r = background.g = background.b = 1.0;
	
	logo = getImageFromAtlas("gfx/main/logo.png", 1);
	
	initBouncers();
	
	initWipe(WIPE_FADE);
	
	showWidgetGroup("title");
	
	getWidget("play", "title")->action = play;
	getWidget("options", "title")->action = options;
	getWidget("stats", "title")->action = stats;
	getWidget("credits", "title")->action = credits;
	getWidget("quit", "title")->action = quit;
	
	app.delegate.logic = &logic;
	app.delegate.draw = &draw;
	app.delegate.postOptions = &postOptions;
	
	if (!isPlayingMusic)
	{
		loadMusic("music/MSTR_-_MSTR_-_Choro_bavario_Loop.ogg");	
		playMusic(0);
		isPlayingMusic = 1;
	}
}

static void initBouncers(void)
{
	int i;
	
	bouncerTypes[0] = getImageFromAtlas("gfx/sprites/yellow1.png", 1);
	bouncerTypes[1] = getImageFromAtlas("gfx/sprites/green1.png", 1);
	bouncerTypes[2] = getImageFromAtlas("gfx/sprites/diamond1.png", 1);
	bouncerTypes[3] = getImageFromAtlas("gfx/sprites/star1.png", 1);
	bouncerTypes[4] = getImageFromAtlas("gfx/sprites/tools.png", 1);
	bouncerTypes[5] = getImageFromAtlas("gfx/sprites/tnt.png", 1);
	bouncerTypes[6] = getImageFromAtlas("gfx/sprites/normalKey.png", 1);
	bouncerTypes[7] = getImageFromAtlas("gfx/sprites/red1.png", 1);
	
	for (i = 0 ; i < MAX_BOUNCERS ; i++)
	{
		bouncers[i].x = rand() % SCREEN_WIDTH - 96;
		bouncers[i].y = SCREEN_HEIGHT;
		bouncers[i].y += rand() % 100;
		bouncers[i].dy = (float) -(4 + rand() % 8);
		bouncers[i].image = bouncerTypes[rand() % MAX_BOUNCER_TYPES];
	}
}

static void logic(void)
{
	int i;
	
	doWipe();
	
	for (i = 0 ; i < MAX_BOUNCERS ; i++)
	{
		bouncers[i].y += bouncers[i].dy;
		bouncers[i].dy += 0.2;

		if (bouncers[i].y >= SCREEN_HEIGHT + 100)
		{
			bouncers[i].x = rand() % SCREEN_WIDTH - 96;
			bouncers[i].dy = -(4 + rand() % 16);
			bouncers[i].image = bouncerTypes[rand() % MAX_BOUNCER_TYPES];
		}
	}
	
	doWidgets();
}

static void draw(void)
{
	int i;
	
	drawBackground(&background);
	
	for (i = 0 ; i < MAX_BOUNCERS ; i++)
	{
		drawGLRectangleBatch(&bouncers[i].image->rect, bouncers[i].x, bouncers[i].y, 0);
	}
	
	drawGLRectangleBatch(&logo->rect, SCREEN_WIDTH / 2, 200, 1);
	
	useFont("cardigan48");
	
	drawWidgets();
	
	useFont("cardigan18");
	
	setGLRectangleBatchColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	drawShadowText(10, SCREEN_HEIGHT - 25, TA_LEFT, "Copyright Parallel Realities, 2016-2018");
	
	drawShadowText(SCREEN_WIDTH - 10, SCREEN_HEIGHT - 25, TA_RIGHT, "Version %.1f.%d", VERSION, REVISION);
	
	drawWipe();
}

static void postOptions(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;
	
	showWidgetGroup("title");
	
	initWipe(WIPE_FADE);
}

static void play(void)
{
	initLevelSelect();
}

static void options(void)
{
	initOptions();
}

static void stats(void)
{
}

static void credits(void)
{
}

static void quit(void)
{
	exit(1);
}
