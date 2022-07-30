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

#include "../common.h"
#include "ending.h"
#include "../system/atlas.h"
#include "../game/title.h"
#include "../system/widgets.h"
#include "../system/text.h"
#include "../system/textures.h"
#include "../system/wipe.h"
#include "../system/draw.h"
#include "../system/sound.h"

#define MAX_BOUNCERS		100
#define MAX_BOUNCER_TYPES	5

extern App app;

static void logic(void);
static void draw(void);
static void ok(void);
static void initBouncers(void);

static Background background;
static Widget *okWidget;
static AtlasImage *bouncerTypes[MAX_BOUNCER_TYPES];
static Bouncer bouncers[MAX_BOUNCERS];

void initEnding(void)
{
	background.texture = loadTexture("gfx/backgrounds/background.jpg")->texture;
	
	showWidgetGroup("ending");
	
	initBouncers();
	
	initWipe(WIPE_FADE);
	
	okWidget = getWidget("ok", "ending");
	okWidget->action = ok;
	
	app.delegate.logic = logic;
	app.delegate.draw = draw;
}

static void initBouncers(void)
{
	int i;
	
	bouncerTypes[0] = getImageFromAtlas("gfx/sprites/diamond1.png", 1);
	bouncerTypes[1] = getImageFromAtlas("gfx/sprites/star1.png", 1);
	bouncerTypes[2] = getImageFromAtlas("gfx/sprites/tools.png", 1);
	bouncerTypes[3] = getImageFromAtlas("gfx/sprites/tnt.png", 1);
	bouncerTypes[4] = getImageFromAtlas("gfx/sprites/normalKey.png", 1);
	
	for (i = 0 ; i < MAX_BOUNCERS ; i++)
	{
		bouncers[i].x = rand() % SCREEN_WIDTH;
		bouncers[i].y = -(rand() % SCREEN_HEIGHT) * 2;
		bouncers[i].dy = rand() % 8;
		bouncers[i].atlasImage = bouncerTypes[rand() % MAX_BOUNCER_TYPES];
	}
}

static void logic(void)
{
	int i;
	
	for (i = 0 ; i < MAX_BOUNCERS ; i++)
	{
		bouncers[i].y += bouncers[i].dy;
		bouncers[i].dy += 0.2;
		if (bouncers[i].dy > 16)
		{
			bouncers[i].dy = 16;
		}

		if (bouncers[i].y >= SCREEN_HEIGHT + 100)
		{
			bouncers[i].x = rand() % SCREEN_WIDTH;
			bouncers[i].y = -(rand() % SCREEN_HEIGHT) * 2;
			bouncers[i].dy = rand() % 8;
			bouncers[i].atlasImage = bouncerTypes[rand() % MAX_BOUNCER_TYPES];
		}
	}
	
	doWidgets();
	
	doWipe();
}

static void draw(void)
{
	int i;
	
	drawBackground(&background);
	
	for (i = 0 ; i < MAX_BOUNCERS ; i++)
	{
		blitAtlasImage(bouncers[i].atlasImage, bouncers[i].x, bouncers[i].y, 1);
	}
	
	drawShadowText(SCREEN_WIDTH / 2, 100, TEXT_ALIGN_CENTER, 60, app.strings[ST_CONGRATULATIONS]);
	
	setTextWidth(700);
	drawShadowText(25, 300, TEXT_ALIGN_LEFT, 45, app.strings[ST_ENDING]);
	setTextWidth(0);
	
	drawWidgets();
	
	drawWipe();
}

static void ok(void)
{
	initTitle();
	
	playSound(SND_SELECT, 0);
}
