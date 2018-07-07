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

#include "ending.h"

static void logic(void);
static void draw(void);
static void ok(void);
static void initBouncers(void);

static Background background;
static Widget *okWidget;
static Atlas *bouncerTypes[MAX_BOUNCER_TYPES];
static Bouncer bouncers[MAX_BOUNCERS];

void initEnding(void)
{
	initGLRectangle(&background.rect, SCREEN_WIDTH, SCREEN_HEIGHT);
	background.rect.texture = loadTexture("gfx/backgrounds/background.jpg")->texture;
	background.r = background.g = background.b = 1.0;
	
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
		bouncers[i].image = bouncerTypes[rand() % MAX_BOUNCER_TYPES];
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
			bouncers[i].image = bouncerTypes[rand() % MAX_BOUNCER_TYPES];
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
		drawGLRectangleBatch(&bouncers[i].image->rect, bouncers[i].x, bouncers[i].y, 1);
	}
	
	drawShadowText(SCREEN_WIDTH / 2, 100, TA_CENTER, 60, "Congratulations!");
	
	setTextWidth(700);
	drawShadowText(25, 300, TA_LEFT, 45, "Thanks to his pals, Red's hunger is finally sated! Unfortunately, he's been away from the office for quite a few hours now, so should probably should get back if he wants to avoid having to work late. Again.");
	setTextWidth(0);
	
	drawWidgets();
	
	drawWipe();
}

static void ok(void)
{
	initTitle();
	
	playSound(SND_SELECT, 0);
}
