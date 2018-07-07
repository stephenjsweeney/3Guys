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

#include "credits.h"

static void logic(void);
static void draw(void);
static void back(void);

static Background background;
static Widget *backWidget;

void initCredits(void)
{
	initGLRectangle(&background.rect, SCREEN_WIDTH, SCREEN_HEIGHT);
	background.rect.texture = loadTexture("gfx/backgrounds/background.jpg")->texture;
	background.r = background.g = background.b = 1.0;
	
	showWidgetGroup("credits");
	
	initWipe(WIPE_FADE);
	
	backWidget = getWidget("back", "credits");
	backWidget->action = back;
	
	app.delegate.logic = logic;
	app.delegate.draw = draw;
}

static void logic(void)
{
	doWidgets();
	
	doWipe();
}

static void draw(void)
{
	drawBackground(&background);
	
	drawText(SCREEN_WIDTH / 2, 100, TA_CENTER, 60, "Credits");
	
	drawText(SCREEN_WIDTH / 2, 250, TA_CENTER, 45, "Sound Effects");
	
	drawText(SCREEN_WIDTH / 2, 350, TA_CENTER, 30, "Created using Bfxr");
	drawText(SCREEN_WIDTH / 2, 400, TA_CENTER, 30, "www.bfxr.net");
	
	drawText(SCREEN_WIDTH / 2, 550, TA_CENTER, 45, "Music");
	
	drawText(SCREEN_WIDTH / 2, 650, TA_CENTER, 30, "Cheerful 1 - Choro Bavario");
	drawText(SCREEN_WIDTH / 2, 700, TA_CENTER, 30, "Copyright 2009 MSTR \"Choro Bavario\"");
	drawText(SCREEN_WIDTH / 2, 750, TA_CENTER, 30, "www.jamendo.com/en/artist/349242/mstr");
	
	drawText(SCREEN_WIDTH / 2, 825, TA_CENTER, 30, "Happy-Go-Lucky Puzzle");
	drawText(SCREEN_WIDTH / 2, 875, TA_CENTER, 30, "Copyright 2014 Axton Crolley");
	
	drawWidgets();
	
	drawWipe();
}

static void back(void)
{
	initTitle();
	
	playSound(SND_SELECT, 0);
}
